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
 *         \file  Tls.c
 *        \brief  Implementation of Tls Module
 *
 *      \details  This file is part of the Tls module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Tls module. >> Tls.h
 *********************************************************************************************************************/

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

#define TLS_SOURCE

/*
abbreviations:
- TLS    - Transport Layer Security
- MAC    - Message AuthentiCation
- IV     - Initialization Vector
- PRF    - PseudoRandom Function (PRF with the SHA and MD5 hash function)
           - An algorithm used to generate keys from the master secret (see Sections 5 and 6.3).

TLS splits in two layers:
TLS_HL: handshake protocol, alert protocol, change cipher spec protocol, application data protocol
TLS_LL: record protocol

 +----------------+------------+-------------------------+-----------------------+
 | handshake prot | alert prot | change cipher spec prot | application data prot |
 +----------------+------------+-------------------------+-----------------------+
 |                                  record prot                                  |
 +-------------------------------------------------------------------------------+

****************************************************************************************************

 +---------------------------------------------------------+
 |                           HTTP                          |
 +---------------------------------------------------------+
                                           |
 +-----------------------------------------|---------------+
 |                                         |           TLS |
 |  +-------------------------+            |               |
 |  | change cipher spec prot |            |               |
 |  +---------------------+---+            |               |
 |  |   handshake prot    | |              |               |
 |  +-----------------+---+ |              |               |
 |  |   alert prot    | |   |              |               |
 |  +-----------------+ |   |              |               |
 |                  |   |   |    +-----------------------+ |
 |                  |   |   |    | application data prot | |
 |                  |   |   |    +-----------------------+ |
 | . . . . . . . . .|. .|. .|. . . . . . . | . . . . . . . |
 |                +-----------------------------------+    |
 |                |            record prot            |    |
 |                +-----------------------------------+    |
 |                                         |               |
 +-----------------------------------------|---------------+
                                           |
 +---------------------------------------------------------+
 |                          TCPIP                          |
 +---------------------------------------------------------+

****************************************************************************************************

TLS Record Protocol:
- The Record Protocol takes messages to be transmitted, fragments the data into manageable blocks, optionally compresses
the data, applies a MAC, encrypts, and transmits the result. Received data is decrypted, verified, decompressed,
reassembled, and then delivered to higher-level clients.
  -> fragment -> compresses -> MAC -> encrypt -> transmit
- MAC(MAC_write_key, seq_num + TLSCompressed.type + TLSCompressed.version + TLSCompressed.length + TLSCompressed.fragment);
- It specifies a compression algorithm, an encryption algorithm, and a MAC algorithm.
- There are always four connection states outstanding: the current read and write states, and the pending read and write
states.

TLSCiphertext.length = TLSCompressed.length plus SecurityParameters.mac_length
Block Ciphers:
- SecurityParameters.record_iv_length (= SecurityParameters.block_size)

RFC 5246: Chapter 9: TLS_RSA_WITH_AES_128_CBC_SHA must be supported (TLS conformance)
*/

/*
  supported chipher suites:
  - TLS_NULL_WITH_NULL_NULL
  - TLS_RSA_WITH_NULL_SHA
  - TLS_RSA_WITH_AES_128_CBC_SHA
  - TLS_RSA_WITH_NULL_SHA256
  - TLS_RSA_WITH_AES_128_CBC_SHA256
  - TLS_ECDH_ECDSA_WITH_NULL_SHA
  - TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA
  - TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
  - TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
  - TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
  - TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
  - TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256

  Record-Layer:
  +-----------+-----------------------+-----------------------+
  | Cont-Type |    Protocol-Version   |    Fragment-Length    |
  |  (1 byte) |        (2 byte)       |        (2 byte)       |
  +-----------+-----------------------+-----------------------+

  ------------------------------------------------
  Handshake-Protocol:
  - (1 byte) Handshake Type
  - (3 byte) Length (bytes, without HandshakeType and Length)
  - < handshake type specific content >

  ------------------
  Client Hello:
  - client_version:      (2 byte)               client (protocol) version
  - random:              (32 byte)              4 byte time stamp UTC, 28 byte random value
  - session_id:          (1 byte + 0..32 byte)  session id length + session id
  - cipher_suites:       (2 byte + x*2 byte)    (2 byte) length (in byte) of following list,
                                                (x*2 (2..2^15-2) byte) chiper suites
  - compression_methods: (1 byte + y*1 byte)    (1 byte) length (in byte) of following list,
                                                (x*1 (1..2^8-1) byte) chiper suites
  - externsions (optional)

  ------------------
  Server Hello:
  - server_version:      (2 byte)               server (protocol) version
  - ... (the same as for client-hello)

  ------------------
  Server Certificate / Client Certificate:
  - certificate_list:    (0..2^24 byte)         sequence of certificates

  ------------------
  Server Key Exchange:
    -> depends on key exchange algorithm
    -> RSA: empty struct -> this whole message will not be used for RSA

  ------------------
  Server Hello Done:
  - empty struct, this message has no content!

  ------------------
  Client Key Exchange:
    -> depends on key exchange algorithm
  - RSA: EncryptedPreMasterSecret
    - client_version:      (2 byte)               client (protocol) version
    - random:              (46 byte)              PreMasterSecret (random value)

  ------------------
  Finished:
  - PRF(xx)
    - xx = master_secret
           + finished_label  (string "client finished" or "server finished")
           + Hash(handshake_messages)

  ------------------

  Connection setup via TLS:
  CLIENT                         SERVER
  ---Client.Hello--------------------->
  <---------------------Server.Hello---
  <---------------Server.Certificate---
  <---------------Server.KeyExchange---
  <--------Server.CertificateRequest---     (X)
  <----------------------Server.Done---
  ---Client.Certificate--------------->     (X)
  ---Client.KeyExchange--------------->
  ---Client.CertificateVerify--------->     (X)
  ---Client.ChangeCipherSuite--------->
  ---Client.Finished------------------>
  <---------Server.ChangeCipherSuite---
  <------------------Server.Finished---

  Messages marked with (X) are only sent if a client certificate shall be used

  TLS 1.0:
  http://tools.ietf.org/html/rfc2246

  TLS 1.2:
  http://tools.ietf.org/html/rfc5246

  AES cipher suites:
  http://tools.ietf.org/html/rfc3268

  Elliptic Curve Cryptography (ECC) Cipher Suites for Transport Layer Security (TLS):
  http://tools.ietf.org/html/rfc4492

  TLS Elliptic Curve Cipher Suites with SHA-256/384 and AES Galois Counter Mode (GCM):
  http://tools.ietf.org/search/rfc5289

  Elliptic Curve Private Key Structure:
  http://tools.ietf.org/html/rfc5915

  Internet X.509 Public Key Infrastructure Certificate and Certificate Revocation List (CRL) Profile:
  http://tools.ietf.org/html/rfc5280

  Certificate signature algorithm ids:
  e.g. Algorithm-ID: 1.2.840.113549.1.1.5
  first level
    * 0 - ITU-T assigned OIDs
    * 1 - ISO assigned OIDs
    * 2 - ISO/ITU-T jointly assigned OIDs
  second level:
    * 1.0 - ISO Standard
    * 1.1 - LDAPv3 "no attributes"
    * 1.2 - ISO member body
    * 1.3 - ISO Identified Organization
  third level:
    * 1.2.156 - China
    * 1.2.276 - Germany (DIN)
    * 1.2.840 - USA
  forth level:
    * 1.2.840.113549  - RSADSI (= RSA Data Security inc.)
  fifth level:
    * 1.2.840.113549.1 - PKCS
    * 1.2.840.113549.2 - PKCS#2
    * 1.2.840.113549.3 - Encryption Algorithms
  sixth level:
    * 1.2.840.113549.1.1  - PKCS-1
    * 1.2.840.113549.1.5  - RSA PKCS 5
    * 1.2.840.113549.1.7  - PKCS-7
    * 1.2.840.113549.1.9  - PKCS-9 - Signatures
    * 1.2.840.113549.1.12 - pkcs-12
    * 1.2.840.113549.1.15 - PKCS#15 Applicatian Identifier
  seventh level:
    * 1.2.840.113549.1.1.1  - RSA encryption
    * 1.2.840.113549.1.1.2  - MD2 with RSA encryption
    * 1.2.840.113549.1.1.3  - iso(1) member-body(2) US(840) rsadsi(113549) pkcs(1) md4WithRSAEncryption(3)
    * 1.2.840.113549.1.1.4  - MD5 with RSA encryption
    * 1.2.840.113549.1.1.5  - SHA-1 with RSA Encryption
    * 1.2.840.113549.1.1.6  - rsaOAEPEncryptionSET
    * 1.2.840.113549.1.1.11 - sha256WithRSAEncryption
    * 1.2.840.113549.1.1.12 - sha384WithRSAEncryption
    * 1.2.840.113549.1.1.13 - sha512WithRSAEncryption
  (info source: 'http://www.alvestrand.no/objectid/1.2.840.113549.1.1.html')

  -> 06 09 2A 86 48 86 F7 0D 01 01 04 :    Md5withRsa
  -> 06 09 2A 86 48 86 F7 0D 01 01 05 :   Sha1withRsa
  -> 06 09 2A 86 48 86 F7 0D 01 01 0B : Sha256withRsa

  ----------------------------------------------------------------------------------------------------
  ISO assigned OIDs (1) -- ISO Identified Organization (3) -- OIW (14) -- secsig (3) -- algorithm  (2)
    * 1.3.14.3.2.25 - md5 With RSA Signature   -> 06 05 2B 0E 03 02 19 :  Md5withRsa
    * 1.3.14.3.2.29 - SHA1 with RSA signature  -> 06 05 2B 0E 03 02 1D : Sha1withRsa

    1.2.840.113549.1.1.4  md5WithRSASignature       (0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x04)
    1.2.840.113549.1.1.5  sha1WithRSASignature      (0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x05)
    1.3.14.3.2.25         md5WithRSASignature       (0x06, 0x05, 0x2B, 0x0E, 0x03, 0x02, 0x19)
    1.3.14.3.2.29         sha1WithRSASignature      (0x06, 0x05, 0x2B, 0x0E, 0x03, 0x02, 0x1D)
    1.2.840.10045.4.1     ecdsaWithSha1Signature    (0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x01)
    1.2.840.10045.4.3.2   ecdsaWithSha256Signature  (0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02)

  elliptic curve id:
  -> 06 08 2A 86 48 CE 3D 03 01 07 : elliptic curve prime256v1 (1.2.840.10045.3.1.7)

    1.3.6.1.5.5.7.48.1.1  id-pkix-ocsp-basic        (0x06, 0x09, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x01)

  OIDs and ASN-coding for hash values with preceding identifier (used for TLS1.2 client_certificate_verify)
  MD5        30 20 30 0c   06 08 2a 86 48 86 f7 0d 02 05 05 00   04 10        1.2.840.113549.2.5
  SHA-1      30 21 30 09   06 05 2b 0e 03 02 1a 05 00   04 14                 1.3.14.3.2.26
  SHA-256    30 31 30 0d   06 09 60 86 48 01 65 03 04 02 01 05 00   04 20     2.16.840.1.101.3.4.2.1
  SHA-384    30 41 30 0d   06 09 60 86 48 01 65 03 04 02 02 05 00   04 30     2.16.840.1.101.3.4.2.2
  SHA-512    30 51 30 0d   06 09 60 86 48 01 65 03 04 02 03 05 00   04 40     2.16.840.1.101.3.4.2.3

  ----------------------------------------------------------------------------------------------------
  OIDs for the content of 'subject' and 'issuer' inside a certificate:
  (see: http://www.alvestrand.no/objectid/2.5.4.html and http://www.oid-info.com/get/2.5.4)
  * 2.5.4.X (X.500 attribute types) -> 55 04
    * 2.5.4.3  - id-at-commonName              -> 55 04 03
    * 2.5.4.6  - id-at-countryName             -> 55 04 06
    * 2.5.4.7  - id-at-localityName            -> 55 04 07
    * 2.5.4.8  - id-at-stateOrProvinceName     -> 55 04 08
    * 2.5.4.10 - id-at-organizationName        -> 55 04 0A
    * 2.5.4.11 - id-at-organizationalUnitName  -> 55 04 0B

  encoding of OID to hex-value:
  http://stackoverflow.com/questions/3376357/how-to-convert-object-identifiers-to-hex-strings

*/

/*lint -e545 */ /* 'Suspicious use of &: This is a legal ANSI C expression' */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Tls.h"

#include "Tls_Priv.h"
#include "Tls_Cbk.h"
#include "IpBase.h"
#include "IpBase_Code.h"
#include "Crypto.h"

#include "TcpIp.h"
#include "NvM.h"

#include "TcpIp_Tcp_Cbk.h"

#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
#include "Scc_Cbk.h"
#endif

#include "vstdlib.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use an include guard */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (TLS_SW_MAJOR_VERSION != 8u) || (TLS_SW_MINOR_VERSION != 2u) || (TLS_SW_PATCH_VERSION != 0u) )
  #error "Tls.c: Source and Header file are inconsistent!"
#endif

/* Check configuration variant for building the library */
#if (defined V_EXTENDED_BUILD_LIB_CHECK)
/* only TLS_CONFIG_VARIANT_PRECOMPILE is implemented */
  #error "Tls.c: Lib check not possible because only variant compile-time is supported!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if defined (STATIC)
#else
# define STATIC static
#endif

#define TLS_DEV_TIME_MEASUREMENT_ENABLED          STD_ON
#define TLS_DEV_TIME_MEASUREMENT_LOOPS_ENABLED    STD_ON

/* TLS version implemented by this component: */
#define TLS_MAJOR_VER          3
#define TLS_MINOR_VER_TLS10    1
#define TLS_MINOR_VER_TLS12    3
#define TLS_PROT_VER_TLS10     0x31
#define TLS_PROT_VER_TLS12     0x33

/* ---------------------------------------------------------------------------------------------------------------------
   ---------------------------------------------------------------------------------------------------------------------
   ---------------------------------------------------------------------------------------------------------------------
   ------------------------------------------------------------------------------------------------------------------ */
/* Values that have to be set manually -----
   This is only a temporary solution, later these values will be configured or read during handshake. Then they can be
   removed here.
*/

#define TLS_ENABLE_SUPPORT_RSA_MD5_CERTS    STD_OFF
#define TLS_ENABLE_STIR_RNG                 STD_ON
#define TLS_DEV_ERROR_REPORT                TLS_DEV_ERROR_DETECT

/* ---------------------------------------------------------------------------------------------------------------------
   ---------------------------------------------------------------------------------------------------------------------
   ---------------------------------------------------------------------------------------------------------------------
   ------------------------------------------------------------------------------------------------------------------ */

/* TCP and TLS states combined in one */
#define TLS_STATE_TCP_OPENING_TLS_CLOSED         1u
#define TLS_STATE_TCP_ESTABL_TLS_OPENING         2u
#define TLS_STATE_TCP_ESTABL_TLS_ESTABL          3u
#define TLS_STATE_TCP_ESTABL_TLS_CLOSING         4u
#define TLS_STATE_TCP_CLOSING_TLS_CLOSED         5u
#define TLS_STATE_TCP_CLOSED_TLS_CLOSED          6u

/* certificate parsing elements (used in CertDesc): */
#define TLS_CERT_ELE_CERT_CH1               0x0001u
#define TLS_CERT_ELE_SIGNED_CERT            0x0002u
#define TLS_CERT_ELE_MY_SIGN_ALG_ID         0x0004u
#define TLS_CERT_ELE_ISSUER                 0x0008u
#define TLS_CERT_ELE_VALID_NOT_BEFORE       0x0010u
#define TLS_CERT_ELE_VALID_NOT_AFTER        0x0020u
#define TLS_CERT_ELE_SUBJECT                0x0040u
#define TLS_CERT_ELE_KEY_ALG_ID             0x0080u
#define TLS_CERT_ELE_KEY_RSA_MODULUS        0x0100u
#define TLS_CERT_ELE_KEY_RSA_PUB_EXP        0x0200u
#define TLS_CERT_ELE_KEY_ECC_CURVE          0x0400u
#define TLS_CERT_ELE_KEY_ECC_PUB_KEY        0x0800u
#define TLS_CERT_ELE_CERT_SIGN_ALG_ID       0x1000u
#define TLS_CERT_ELE_CERT_SIGNATURE         0x2000u
#define TLS_CERT_ELE_SERIAL_NUMBER          0x4000u

/* RSA private key elements */
#define TLS_RSA_KEY_NUM_ELEMENTS                 5u

#define TLS_ENC_STATE_IDLE                       0u  /* encryption idle */
#define TLS_ENC_STATE_RUNNING                    1u  /* encryption running */

/* Alert Protocol --------------------------------------- */
/* typedef uint8 Tls_AlertLevelType; */
#define TLS_ALERT_LEVEL_WARNING               0x01u
#define TLS_ALERT_LEVEL_FATAL                 0x02u

/* typedef uint8 Tls_AlertDescrType; */  /* AlertDescription */
/* Alerts marked with and 'f' are FATALE errors */
#define TLS_ALERT_DESC_TCP_FIN                 254u /* TCP FIN has to be sent - VECTOR MODULE INTERNAL STATE */
#define TLS_ALERT_DESC_NONE                    255u /* reset value for internal variable */
#define TLS_ALERT_DESC_CLOSE_NOTIFY              0u /* close_notify */
#define TLS_ALERT_DESC_UNEXP_MSG                10u /* f, unexpected_message */
#define TLS_ALERT_DESC_BAD_RECORD_MAC           20u /* f, bad_record_mac */
#define TLS_ALERT_DESC_DECR_FAILED_RESERVED     21u /* decryption_failed_RESERVED - old */
#define TLS_ALERT_DESC_RECORD_OVERFLOW          22u /* f, record_overflow */
#define TLS_ALERT_DESC_DECOMP_FAILURE           30u /* f, decompression_failure */
#define TLS_ALERT_DESC_HANDSHAKE_FAILURE        40u /* f, handshake_failure */
#define TLS_ALERT_DESC_NO_CERT_RESERVED         41u /* no_certificate_RESERVED - old */
#define TLS_ALERT_DESC_BAD_CERT                 42u /* bad_certificate */
#define TLS_ALERT_DESC_UNSUP_CERT               43u /* unsupported_certificate */
#define TLS_ALERT_DESC_CERT_REVOKED             44u /* certificate_revoked */
#define TLS_ALERT_DESC_CERT_EXPIRED             45u /* certificate_expired */
#define TLS_ALERT_DESC_CERT_UNKNOWN             46u /* certificate_unknown */
#define TLS_ALERT_DESC_ILLEGAL_PARAM            47u /* f, illegal_parameter */
#define TLS_ALERT_DESC_UNKNOWN_CA               48u /* f, unknown_ca */
#define TLS_ALERT_DESC_ACCESS_DENIED            49u /* f, access_denied */
#define TLS_ALERT_DESC_DECODE_ERROR             50u /* f, decode_error */
#define TLS_ALERT_DESC_DECRYPT_ERROR            51u /* f, decrypt_error */
#define TLS_ALERT_DESC_EXP_RESTR_FAILURE        60u /* export_restriction_RESERVED - old */
#define TLS_ALERT_DESC_PROT_VERSION             70u /* f, protocol_version */
#define TLS_ALERT_DESC_INSUF_SEC                71u /* f, insufficient_security */
#define TLS_ALERT_DESC_INT_ERROR                80u /* f, internal_error */
#define TLS_ALERT_DESC_USER_CANCELED            90u /* user_canceled */
#define TLS_ALERT_DESC_NO_RENEGOT              100u /* no_renegotiation */
#define TLS_ALERT_DESC_UNSUP_EXT               110u /* f, unsupported_extension */
#define TLS_ALERT_DESC_CERT_UNOBTAINABLE       111u /* f, certificate_unobtainable */
#define TLS_ALERT_DESC_UNRECOGNIZED_NAME       112u /* f, unrecognized_name */
#define TLS_ALERT_DESC_BAD_CERT_STATUS_RESP    113u /* f, bad_certificate_status_response */
#define TLS_ALERT_DESC_BAD_CERT_HASH_VALUE     114u /* f, bad_certificate_hash_value */

/* bulk_cipher_algorithm used in Tls_SecurityParametersType */
#define TLS_BULK_CIPHER_ALGORITHM_NULL        0x00u
#define TLS_BULK_CIPHER_ALGORITHM_RC4         0x01u
#define TLS_BULK_CIPHER_ALGORITHM_3DES        0x02u
#define TLS_BULK_CIPHER_ALGORITHM_AES128_CBC  0x03u
#define TLS_BULK_CIPHER_ALGORITHM_AES256_CBC  0x04u
#define TLS_BULK_CIPHER_ALGORITHM_AES128_GCM  0x05u
#define TLS_BULK_CIPHER_ALGORITHM_AES256_GCM  0x06u
/* cipher_type used in Tls_SecurityParametersType */
#define TLS_CIPHER_TYPE_STREAM                0x00u
#define TLS_CIPHER_TYPE_BLOCK                 0x01u
#define TLS_CIPHER_TYPE_AEAD                  0x02u
/* mac_algorithm used in Tls_SecurityParametersType */
#define TLS_MAC_ALGORITHM_NULL                0x00u
#define TLS_MAC_ALGORITHM_HMAC_MD5            0x01u
#define TLS_MAC_ALGORITHM_HMAC_SHA1           0x02u
#define TLS_MAC_ALGORITHM_HMAC_SHA224         0x03u
#define TLS_MAC_ALGORITHM_HMAC_SHA256         0x04u
#define TLS_MAC_ALGORITHM_HMAC_SHA384         0x05u
#define TLS_MAC_ALGORITHM_HMAC_SHA512         0x06u
#define TLS_MAC_ALGORITHM_AEAD_SHA256         0x07u  /* eg. AES128_GCM_SHA256 */
/* compression_algorithm used in Tls_SecurityParametersType */
#define TLS_COMPR_ALGORITHM_NULL              0x00u

/* HashAlgorithms and SignatureAlgorithms as defined in RFC5246 (TLS 1.2), section 7.4.1.4.1. Signature Algorithms */
/* HashAlgorithm     : none(0), md5(1), sha1(2), sha224(3), sha256(4), sha384(5), sha512(6), (255) */
/* SignatureAlgorithm: anonymous(0), rsa(1), dsa(2), ecdsa(3), (255) */
#define TLS_HASH_ALGORITHM_NONE               0x00u
#define TLS_HASH_ALGORITHM_MD5                0x01u
#define TLS_HASH_ALGORITHM_SHA1               0x02u
#define TLS_HASH_ALGORITHM_SHA224             0x03u
#define TLS_HASH_ALGORITHM_SHA256             0x04u
#define TLS_HASH_ALGORITHM_SHA384             0x05u
#define TLS_HASH_ALGORITHM_SHA512             0x06u

#define TLS_SIGN_ALGORITHM_ANONYMOUS          0x00u
#define TLS_SIGN_ALGORITHM_RSA                0x01u
#define TLS_SIGN_ALGORITHM_DSA                0x02u
#define TLS_SIGN_ALGORITHM_ECDSA              0x03u

#define TLS_ECC_P256R1_COORDINATE_SIZE          32u

#define TLS_OID_OCSP_RESP_TYPE_OCSP_BASIC   {0x06, 0x09, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x01}  /* 1.3.6.1.5.5.7.48.1.1  id-pkix-ocsp-basic */

/* Handshake Protocol ----------------------------------- */
 /* typedef uint8 Tls_HandShakeMsgType; */
#define TLS_HS_TYPE_HELLO_REQUEST                0u /* hello_request */
#define TLS_HS_TYPE_CLIENT_HELLO                 1u /* client_hello */
#define TLS_HS_TYPE_SERVER_HELLO                 2u /* server_hello */
#define TLS_HS_TYPE_CERTIFICATE                 11u /* certificate */
#define TLS_HS_TYPE_SERVER_KEY_EXCH             12u /* server_key_exchange */
#define TLS_HS_TYPE_CERT_REQUEST                13u /* certificate_request */
#define TLS_HS_TYPE_SERVER_HELLO_DONE           14u /* server_hello_done */
#define TLS_HS_TYPE_CERT_VERIFY                 15u /* certificate_verify */
#define TLS_HS_TYPE_CLIENT_KEY_EXCH             16u /* client_key_exchange */
#define TLS_HS_TYPE_FINISHED                    20u /* finished */
#define TLS_HS_TYPE_CERT_STATUS                 22u /* certificate status */

/* values for Tls_HandShakeState */
#define TLS_HS_STATE_INIT                        1u
#define TLS_HS_STATE_HELLO_REQUESTED             2u
#define TLS_HS_STATE_HELLO_SENT                  3u
#define TLS_HS_STATE_HELLO_REC                   4u
#define TLS_HS_STATE_CERT_REC                    5u
#define TLS_HS_STATE_CERT_STATUS_REC             6u
#define TLS_HS_STATE_CERT_REQUESTED              7u
#define TLS_HS_STATE_HELLO_DONE_REC              8u
#define TLS_HS_STATE_CERT_SENT                   9u
#define TLS_HS_STATE_CL_KEY_EXCH_SENT           10u
#define TLS_HS_STATE_CERT_VERI_SENT             11u
#define TLS_HS_STATE_CCS_SENT                   12u  /* CCS = change cipher specs */
#define TLS_HS_STATE_FINISHED_SENT              13u
#define TLS_HS_STATE_CCS_REC                    14u
#define TLS_HS_STATE_FINISHED_REC               15u
#define TLS_HS_STATE_CONN_ESTABLISHED           16u  /* handshake completed */

/* handshake message defines */
#define TLS_HS_OFFSET_HS_TYPE                      0u
#define TLS_HS_OFFSET_LEN_FIELD                    1u

/* client hello defines */
#define TLS_HS_C_HELLO_OFFSET_MAJOR_VER            4u
#define TLS_HS_C_HELLO_OFFSET_MINOR_VER            5u
#define TLS_HS_C_HELLO_OFFSET_RAND                 6u
#define TLS_HS_C_HELLO_OFFSET_SESSION_ID_LEN      38u
#define TLS_HS_C_HELLO_OFFSET_CIPHER_SUITE_LIST   39u
#define TLS_HS_C_HELLO_TIME_LEN                    4u
#define TLS_HS_HELLO_EXTENSION_TYPE_LEN            2u
#define TLS_HS_HELLO_EXTENSION_LEN_LEN             2u
#define TLS_HS_HELLO_OFFSET_EXTENSION_CONTENT      4u

/* client certificate verify defines */
#define TLS_CERT_VERI_RSA_SHA256_SIGN_DESCR_LEN   19u
#define TLS_HS_C_CERT_VERI_TLS12_EXTRA_OFFSET           2u
#define TLS_HS_C_CERT_VERI_TLS10_OFFSET_SIGN_TOT_LEN    4u
#define TLS_HS_C_CERT_VERI_OFFSET_HASH_ALG         4u
#define TLS_HS_C_CERT_VERI_OFFSET_SIGN_ALG         5u

/* certificate parsing defines */
#define TLS_HS_CERT_PARSING_MAX_ELE_DEPTH          5u
#define TLS_HS_CERT_PARSING_DEPTH_ONE              1u
#define TLS_HS_CERT_PARSING_DEPTH_TWO              2u
#define TLS_HS_CERT_PARSING_DEPTH_SERIAL           3u

/* heartbeat defines */
#define TLS_HB_OFFSET_MSG_TYPE                     0u
#define TLS_HB_OFFSET_PAYLOAD_LEN                  1u
#define TLS_HB_OFFSET_PAYLOAD                      3u

/* TLS hello extensions */
#define TLS_HS_HELLO_EXT_TRUSTED_CA_KEYS         3u
#define TLS_HS_HELLO_EXT_OCSP_STATUS_REQUEST     5u
#define TLS_HS_HELLO_EXT_ELLIPTIC_CURVES        10u
#define TLS_HS_HELLO_EXT_EC_POINT_FORMATS       11u
#define TLS_HS_HELLO_EXT_SIGN_ALGORITHMS        13u
#define TLS_HS_HELLO_EXT_HEARTBEAT              15u
#define TLS_HS_HELLO_EXT_OCSP_STATUS_REQUEST_V2 17u

/* IdentifierType used in the 'Trusted CA Indication' extension of the client_hello (RFC 6066) */
#define TLS_EXT_TRUSTED_CA_IDENT_TYPE_PRE_AGREED        0u
#define TLS_EXT_TRUSTED_CA_IDENT_TYPE_KEY_SHA1_HASH     1u
#define TLS_EXT_TRUSTED_CA_IDENT_TYPE_X509_NAME         2u
#define TLS_EXT_TRUSTED_CA_IDENT_TYPE_CERT_SHA1_HASH    3u

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
#define TLS_HB_MSG_HEARTBEAT_REQUEST             1u  /* heartbeat_request */
#define TLS_HB_MSG_HEARTBEAT_RESPONSE            2u  /* heartbeat_response */

#endif

/* TLS content types */
#define TLS_REC_CONTENT_CCS                     20u /* change_cipher_spec */
#define TLS_REC_CONTENT_A                       21u /* alert */
#define TLS_REC_CONTENT_HS                      22u /* handshake */
#define TLS_REC_CONTENT_AD                      23u /* application_data */
#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
#define TLS_REC_CONTENT_HB                      24u /* heartbeat */
#endif

#define TLS_RL_HDR_LEN                           5u
#define TLS_HS_HDR_LEN                           4u

#define TLS_RL_OFFSET_MSG_TYPE                   0u
#define TLS_RL_OFFSET_PROT_VER                   1u
#define TLS_RL_OFFSET_MSG_LEN                    3u
#define TLS_RL_SEQUENCE_NUM_LEN                  8u

#define TLS_PROT_VER_LEN                         2u
#define TLS_AL_MSG_LEN                           2u
#define TLS_C_HELLO_RAND_LEN                    32u
#define TLS_ECDH_SECRET_LEN_XY_256BIT           64u

/* used for Tls_HsHashAlg[] */
#define TLS_HS_HASH_ALG_MD5SHA1                  1u
#define TLS_HS_HASH_ALG_SHA256                   2u

/* lengths and positions in key material: MAC + KEY + IV */
#define TLS_KEY_PART_OFFSET_ALL_MAC              0u
#define TLS_KEY_PART_OFFSET_AES128_SHA1_KEY     20u  /* eg 002F, C004 */
#define TLS_KEY_PART_OFFSET_AES128_SHA256_KEY   32u  /* eg 003C, C025 */
#define TLS_KEY_PART_OFFSET_AES128_SHA1_IV      36u
#define TLS_KEY_PART_OFFSET_AES128_SHA256_IV    48u
#define TLS_KEY_PART_OFFSET_AES128_GCM_KEY       0u
#define TLS_KEY_PART_OFFSET_AES128_GCM_IV       16u

#define TLS_AES128_KEY_LEN                      16u
#define TLS_AES_BLOCK_LEN                       16u  /* the initialization vector (IV) always has the same length as the block */

#define TLS_AES_GCM_FIXED_IV_LENGTH              4u  /* nonce implicit */
#define TLS_AES_GCM_RECORD_IV_LENGTH             8u  /* nonce explicit */
#define TLS_AES_GCM_TOTAL_IV_LENGTH             12u  /* nonce explicit + nonce implicit */
#define TLS_AES_GCM_AUTH_DATA_LEN               13u
#define TLS_AES_GCM_AD_EXTRA_LEN                 2u  /* extra length field for AES GCM application data */

/* key exchange type, used in Tls_Hs_KeyExchType[] */
#define TLS_KEY_EXCH_TYPE_RSA                 0x01u
#define TLS_KEY_EXCH_TYPE_ECDH                0x02u
#define TLS_KEY_EXCH_TYPE_ECDHE               0x03u

#define TLS_ECC_PUB_KEY_POINT_FORMAT_UNCOMPRESSED     0x04u
#define TLS_HELLO_EXT_ECC_POINT_FORMAT_UNCOMPRESSED   0x00u

/* key mat size = 2*(HMAC + AES-KEY + AES-IV) */
#define TLS_KEY_BLOCK_SIZE_NULL_SHA1            40u  /* eg 0002, C001 */
#define TLS_KEY_BLOCK_SIZE_NULL_SHA256          64u  /* eg 003B */
#define TLS_KEY_BLOCK_SIZE_AES128_SHA1         104u  /* eg 002F, C004 */
#define TLS_KEY_BLOCK_SIZE_AES128_SHA256       128u  /* eg 003C, C025 */
#define TLS_KEY_BLOCK_SIZE_AES128_GCM          (2*(16 + 4))      /* AES-KEY + SALT (The salt length (SecurityParameters.fixed_iv_length) is 4 octets.) */
#define TLS_PRF_MAX_LEN                        148u  /* max buffer len: min = prf_max_len, max = min + 32 (longest prf hash fct (sha256)) */
#define TLS_PRF_MAX_BUFFER_LEN                 (TLS_PRF_MAX_LEN + TLS_HASH_SHA256_LEN)  /* max buffer len: min = prf_max_len, max = min + 32 (longest prf hash fct) */

#define TLS_LABEL_CLIENT_FINISHED_LEN           15u
#define TLS_LABEL_SERVER_FINISHED_LEN           15u
#define TLS_LABEL_MASTER_SECRET_LEN             13u
#define TLS_LABEL_KEY_EXPENSION_LEN             13u
#define TLS_MASTER_SECRET_LEN                   48u
#define TLS_HS_RANDOM_LEN                       32u
#define TLS_MAX_SUPP_RSA_KEY_LEN_BYTE          256u
#define TLS_MAX_SUPP_ECC_KEY_COMP_SIZE          32u

#define TLS_HS_CLIENT_CERT_RSA_SIGN               0x01u
#define TLS_HS_CLIENT_CERT_DSS_SIGN               0x02u
#define TLS_HS_CLIENT_CERT_RSA_FIXED_DH           0x03u
#define TLS_HS_CLIENT_CERT_DSS_FIXED_DH           0x04u
#define TLS_HS_CLIENT_CERT_ECDSA_SIGN             0x40u  /* ClientCertificateTypes introduced in RFC 4492 */
#define TLS_HS_CLIENT_CERT_RSA_FIXED_ECDH_SIGN    0x41u
#define TLS_HS_CLIENT_CERT_ECDSA_FIXED_ECDH_SIGN  0x42u

/* Basic Encoding Rules (BER) defines */
#define TLS_BER_TYPE_PRIMITIVE_INTEGER            0x02u
#define TLS_BER_TYPE_PRIMITIVE_BIT_STRING         0x03u
#define TLS_BER_TYPE_PRIMITIVE_OCTET_STRING       0x04u
#define TLS_BER_TYPE_CONSTRUCTED_SEQUENCE         0x30u

/* states and lengths inside TLS elements and messages ------------------------------------------ */

/* RL status, used in Tls_Rl_CurrentPart[] */
#define TLS_RL_IN_HDR                         0x01u
#define TLS_RL_IN_DATA                        0x02u
#define TLS_RL_IN_IGNORE                      0x03u

/* RX states inside any of the layers above the RL */
#define TLS_UL_IN_FIRST_ELEMENT               0x01u

/* in handshake messages ----------------- */
/* RX states inside HS (all message types): */
#define TLS_HS_S_IN_TYPE                      0x01u
#define TLS_HS_S_IN_LEN                       0x02u
#define TLS_HS_S_IN_GENERIC_3                 0x03u
/* RX states inside HS Server Hello: */
#define TLS_HS_S_HELLO_IN_VERSION             0x03u
#define TLS_HS_S_HELLO_IN_RANDOM              0x04u
#define TLS_HS_S_HELLO_IN_SID_LEN             0x05u
#define TLS_HS_S_HELLO_IN_SID                 0x06u
#define TLS_HS_S_HELLO_IN_CIPHER_S            0x07u
#define TLS_HS_S_HELLO_IN_COMP_M              0x08u
/* RX states inside HS Server Certificate: */
#define TLS_HS_S_CERT_IN_ALL_CERTS_LEN        0x03u
#define TLS_HS_S_CERT_IN_CERT_DATA            0x04u
/* RX states inside HS Server Hello Done: */
  /* 'Hello Done' never contains any content */

/* RX length of elements inside HS (all message types): */
#define TLS_HS_S_TYPE_LEN                        1u
#define TLS_HS_S_LEN_LEN                         3u
#define TLS_HS_PREFIX_LEN                       (TLS_HS_S_TYPE_LEN + TLS_HS_S_LEN_LEN)
/* RX length of elements inside HS Server Hello: */
#define TLS_HS_S_HELLO_VERSION_LEN               2u
#define TLS_HS_S_HELLO_RANDOM_LEN               32u
#define TLS_HS_S_HELLO_SID_LEN_LEN               1u
#define TLS_HS_S_HELLO_CIPHER_S_LEN              2u
#define TLS_HS_S_HELLO_COMP_M_LEN                1u
/* RX length of elements inside HS Server Certificate: */
#define TLS_HS_S_CERT_CERTS_LEN_LEN              3u
#define TLS_HS_S_TLS_CERT_LEN_LEN                3u
/* RX length of elements inside HS Server Hello Done: */
  /* n.a. */
/* length of client or server finished message */
#define TLS_HS_X_FINISHED_LEN                   12u

/* state used in Tls_ConnNvmRequStatus[]:
   - IDLE  : currently no block is requested, a new block can be requested
   - PEND  : a block is requested, no further request shall be issued
   - READY : a block is read from NVM an can be used in the appl. Set state to IDLE when block is not needed any longer.
   - BUSY  : a block is waiting for being requested */
#define TLS_NVM_REQU_IDLE                        0u
#define TLS_NVM_REQU_RETRY                       1u
#define TLS_NVM_REQU_PEND                        2u
#define TLS_NVM_REQU_READY                       3u
#define TLS_NVM_REQU_BUSY                        4u

#define TLS_NVM_ACCESS_IDLE                      0u
#define TLS_NVM_ACCESS_IN_USE                    1u

#define TLS_NVM_INV_BLOCK_ID           ((NvM_BlockIdType)(0xFFFFu))

/* states used in Tls_RxBlockingState:
   - NONE      : currently no decrypted data is stored in RxPlainTextBuffer
   - LOCKED    : currently a RL element is in the plaintext buffer and is beeing processed and has to wait for some event (NVM read or crypto calculation finished). Prevent the module from trying to analyse further elements.
   - UNLOCKING : Data could be read from NVM, now proceed with the data handling */
#define TLS_RX_BLOCKING_STATE_NONE               0u
#define TLS_RX_BLOCKING_STATE_LOCKED             1u
#define TLS_RX_BLOCKING_STATE_UNLOCKING          2u

/* state used in Tls_ServCertsParseStatus: */
/* state of parsing all stored server certs */
#define TLS_SERV_CERT_PARSE_NONE                 0u
#define TLS_SERV_CERT_PARSE_ACTIVE               1u
#define TLS_SERV_CERT_PARSE_COMPLETE             2u
#define TLS_SERV_CERT_PARSE_FAILED               3u

/* state used in Tls_CrlParseStatus: */
/* state of parsing all stored CRLs */
#define TLS_CRL_PARSE_NONE                       0u
#define TLS_CRL_PARSE_ACTIVE                     1u
#define TLS_CRL_PARSE_COMPLETE                   2u
#define TLS_CRL_PARSE_FAILED                     3u

/* HashAlgorithm     : none(0), md5(1), sha1(2), sha224(3), sha256(4), sha384(5), sha512(6), (255) */
/* SignatureAlgorithm: anonymous(0), rsa(1), dsa(2), ecdsa(3), (255) */
#define TLS_HASH_SIGN_ALG_ID_MD5_RSA               0x0101u
#define TLS_HASH_SIGN_ALG_ID_SHA1_RSA              0x0201u
#define TLS_HASH_SIGN_ALG_ID_SHA256_RSA            0x0401u
#define TLS_HASH_SIGN_ALG_ID_SHA1_ECDSA            0x0203u
#define TLS_HASH_SIGN_ALG_ID_SHA256_ECDSA          0x0403u
#define TLS_HASH_SIGN_ALG_ID_INVALID               0xFFFFu

/* hash elements can be combined -> values used as flag */
#define TLS_HASH_INFO_NONE                    0x00u
#define TLS_HASH_INFO_ISSUER                  0x01u
#define TLS_HASH_INFO_SUBJECT                 0x02u

/* in alert messages --------------------- */
/* RX states inside Alert: */
#define TLS_AL_IN_LEVEL                       0x01u
#define TLS_AL_IN_TYPE                        0x02u

#define TLS_UINT8_INV_VALUE                   0xFFu
#define TLS_UINT16_INV_VALUE                0xFFFFu
#define TLS_UINT8_MAX_VALUE                   0xFFu
#define TLS_UINT16_MAX_VALUE                0xFFFFu
#define TLS_UINT32_MAX_VALUE            0xFFFFFFFFu

/* CertificateStatusType, see RFC 6960 and 6961 */
#define TLS_CERT_STAUS_TYPE_OCSP              0x01U
#define TLS_CERT_STAUS_TYPE_OCSP_MULTI        0x02U

/* CRL search status */
#define TLS_CRL_SEARCH_STATUS_CERT_REVOKED       1U  /* NOT OK */
#define TLS_CRL_SEARCH_STATUS_INT_ERROR          2U  /* NOT OK */
#define TLS_CRL_SEARCH_STATUS_TOO_MANY_CERTS     3U  /* NOT OK */
#define TLS_CRL_SEARCH_STATUS_CERT_NOT_REVOKED   4U  /* OK */

/* Heartbeat */
#define TLS_HEARTBEAT_MIN_PADDING_SIZE           16U

/* ---------------------------------------------------------------------------------------------- */

#ifdef TLS_PROCESSOR_CANOEEMU
  #define TLS_VENABLE_CANOE_WRITE_STRING     STD_OFF
#else
  #define TLS_VENABLE_CANOE_WRITE_STRING    STD_OFF
#endif
  /* TLS_PROCESSOR_CANOEEMU */

#define TLS_LOG_TIME_CRYPT_SUPPORT     STD_OFF
#define TLS_LOG_TIME_DIV_SUPPORT       STD_OFF
#define TLS_LOG_DIV_SUPPORT            STD_ON
#define TLS_LOG_HS_DIV_SUPPORT         STD_ON
#define TLS_LOG_HS_CRYPT_SUPPORT       STD_ON
#define TLS_LOG_RX_IND_SUPPORT         STD_ON
#define TLS_LOG_RL_SUPPORT             STD_ON
#define TLS_LOG_ALERT_SUPPORT          STD_ON

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
#if ((defined TLS_PROCESSOR_CANOEEMU) && (TLS_VENABLE_CANOE_WRITE_STRING == STD_ON))
  #include "stdio.h"

  #define CANOE_WRITE_STRING(Txt)                 CANoeAPI_WriteString((Txt));
  #define CANOE_WRITE_STRING_1(Txt, P1)           \
    _snprintf_s(Tls_CanoeWriteStr, 256, 256, Txt, P1); CANoeAPI_WriteString((Tls_CanoeWriteStr));  /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #define CANOE_WRITE_STRING_2(Txt, P1, P2)       \
    _snprintf_s(Tls_CanoeWriteStr, 256, 256, Txt, P1, P2); \
    CANoeAPI_WriteString((Tls_CanoeWriteStr));  /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #define CANOE_WRITE_STRING_3(Txt, P1, P2, P3)   \
    _snprintf_s(Tls_CanoeWriteStr, 256, 256, Txt, P1, P2, P3); \
    CANoeAPI_WriteString((Tls_CanoeWriteStr));  /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #define CANOE_WRITE_STRING_4(Txt, P1, P2, P3, P4)   \
    _snprintf_s(Tls_CanoeWriteStr, 256, 256, Txt, P1, P2, P3, P4); \
    CANoeAPI_WriteString((Tls_CanoeWriteStr));  /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #define CANOE_WRITE_STRING_8(Txt, P1, P2, P3, P4, P5, P6, P7, P8)   \
    _snprintf_s(Tls_CanoeWriteStr, 256, 256, Txt, P1, P2, P3, P4, P5, P6, P7, P8); \
    CANoeAPI_WriteString((Tls_CanoeWriteStr));  /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #define CANOE_WRITE_STRING_ARRAY(ArrayPtr, ArrayLen)   \
    Tls_VConvertByteArray2HexString(ArrayPtr, ArrayLen); \
    CANoeAPI_WriteString((Tls_CanoeWriteStr));  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#else
  #define CANOE_WRITE_STRING(txt)
  #define CANOE_WRITE_STRING_1(txt, p1)
  #define CANOE_WRITE_STRING_2(txt, p1, p2)
  #define CANOE_WRITE_STRING_3(txt, p1, p2, p3)
  #define CANOE_WRITE_STRING_4(Txt, P1, P2, P3, P4)
  #define CANOE_WRITE_STRING_8(Txt, P1, P2, P3, P4, P5, P6, P7, P8)
  #define CANOE_WRITE_STRING_ARRAY(ArrayPtr, ArraySize)
#endif
  /* TLS_PROCESSOR_CANOEEMU && X_VENABLE_CANOE_WRITE_STRING */

#if (TLS_LOG_TIME_CRYPT_SUPPORT == STD_ON)
#define TLS_LOG_TIME_CRYPT(txt) \
  CANOE_WRITE_STRING(txt)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_TIME_CRYPT_1(txt, p1) \
  CANOE_WRITE_STRING_1(txt, p1)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_TIME_CRYPT_2(txt, p1, p2) \
  CANOE_WRITE_STRING_2(txt, p1, p2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_TIME_CRYPT_3(txt, p1, p2, p3) \
  CANOE_WRITE_STRING_3(txt, p1, p2, p3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TLS_LOG_TIME_CRYPT(txt)
#define TLS_LOG_TIME_CRYPT_1(txt, p1)
#define TLS_LOG_TIME_CRYPT_2(txt, p1, p2)
#endif

#if (TLS_LOG_TIME_DIV_SUPPORT == STD_ON)
#define TLS_LOG_TIME_DIV(txt) \
  CANOE_WRITE_STRING(txt)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_TIME_DIV_1(txt, p1) \
  CANOE_WRITE_STRING_1(txt, p1)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_TIME_DIV_2(txt, p1, p2) \
  CANOE_WRITE_STRING_2(txt, p1, p2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_TIME_DIV_3(txt, p1, p2, p3) \
  CANOE_WRITE_STRING_3(txt, p1, p2, p3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TLS_LOG_TIME_DIV(txt)
#define TLS_LOG_TIME_DIV_1(txt, p1)
#define TLS_LOG_TIME_DIV_2(txt, p1, p2)
#endif

#if (TLS_LOG_DIV_SUPPORT == STD_ON)
#define TLS_LOG_DIV(txt) \
  CANOE_WRITE_STRING(txt)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_DIV_1(txt, p1) \
  CANOE_WRITE_STRING_1(txt, p1)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_DIV_2(txt, p1, p2) \
  CANOE_WRITE_STRING_2(txt, p1, p2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_DIV_3(txt, p1, p2, p3) \
  CANOE_WRITE_STRING_3(txt, p1, p2, p3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_DIV_4(txt, p1, p2, p3, p4) \
  CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TLS_LOG_DIV(txt)
#define TLS_LOG_DIV_1(txt, p1)
#define TLS_LOG_DIV_2(txt, p1, p2)
#define TLS_LOG_DIV_3(txt, p1, p2, p3)
#define TLS_LOG_DIV_4(txt, p1, p2, p3, p4)
#endif

#if (TLS_LOG_HS_DIV_SUPPORT == STD_ON)
#define TLS_LOG_HS_DIV(txt) \
  CANOE_WRITE_STRING(txt)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_DIV_1(txt, p1) \
  CANOE_WRITE_STRING_1(txt, p1)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_DIV_2(txt, p1, p2) \
  CANOE_WRITE_STRING_2(txt, p1, p2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_DIV_3(txt, p1, p2, p3) \
  CANOE_WRITE_STRING_3(txt, p1, p2, p3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_DIV_4(txt, p1, p2, p3, p4) \
  CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_DIV_8(txt, p1, p2, p3, p4, p5, p6, p7, p8) \
  CANOE_WRITE_STRING_8(txt, p1, p2, p3, p4, p5, p6, p7, p8)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TLS_LOG_HS_DIV(txt)
#define TLS_LOG_HS_DIV_1(txt, p1)
#define TLS_LOG_HS_DIV_2(txt, p1, p2)
#define TLS_LOG_HS_DIV_3(txt, p1, p2, p3)
#define TLS_LOG_HS_DIV_4(txt, p1, p2, p3, p4)
#define TLS_LOG_HS_DIV_8(txt, p1, p2, p3, p4, p5, p6, p7, p8)
#endif

#if (TLS_LOG_HS_CRYPT_SUPPORT == STD_ON)
#define TLS_LOG_HS_CRYPT(txt) \
  CANOE_WRITE_STRING(txt)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_CRYPT_1(txt, p1) \
  CANOE_WRITE_STRING_1(txt, p1)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_CRYPT_2(txt, p1, p2) \
  CANOE_WRITE_STRING_2(txt, p1, p2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_CRYPT_3(txt, p1, p2, p3) \
  CANOE_WRITE_STRING_3(txt, p1, p2, p3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_HS_CRYPT_4(txt, p1, p2, p3, p4) \
  CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TLS_LOG_HS_CRYPT(txt)
#define TLS_LOG_HS_CRYPT_1(txt, p1)
#define TLS_LOG_HS_CRYPT_2(txt, p1, p2)
#define TLS_LOG_HS_CRYPT_3(txt, p1, p2, p3)
#define TLS_LOG_HS_CRYPT_4(txt, p1, p2, p3, p4)
#endif

#if (TLS_LOG_RX_IND_SUPPORT == STD_ON)
#define TLS_LOG_RX_IND(txt) \
  CANOE_WRITE_STRING(txt)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_RX_IND_1(txt, p1) \
  CANOE_WRITE_STRING_1(txt, p1)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_RX_IND_2(txt, p1, p2) \
  CANOE_WRITE_STRING_2(txt, p1, p2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_RX_IND_3(txt, p1, p2, p3) \
  CANOE_WRITE_STRING_3(txt, p1, p2, p3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_RX_IND_4(txt, p1, p2, p3, p4) \
  CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TLS_LOG_RX_IND(txt)
#define TLS_LOG_RX_IND_1(txt, p1)
#define TLS_LOG_RX_IND_2(txt, p1, p2)
#define TLS_LOG_RX_IND_3(txt, p1, p2, p3)
#define TLS_LOG_RX_IND_4(txt, p1, p2, p3, p4)
#endif

#if (TLS_LOG_RL_SUPPORT == STD_ON)
#define TLS_LOG_RL(txt) \
  CANOE_WRITE_STRING(txt)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_RL_1(txt, p1) \
  CANOE_WRITE_STRING_1(txt, p1)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_RL_2(txt, p1, p2) \
  CANOE_WRITE_STRING_2(txt, p1, p2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_RL_3(txt, p1, p2, p3) \
  CANOE_WRITE_STRING_3(txt, p1, p2, p3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_RL_4(txt, p1, p2, p3, p4) \
  CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TLS_LOG_RL(txt)
#define TLS_LOG_RL_1(txt, p1)
#define TLS_LOG_RL_2(txt, p1, p2)
#define TLS_LOG_RL_3(txt, p1, p2, p3)
#define TLS_LOG_RL_4(txt, p1, p2, p3, p4)
#endif

#if (TLS_LOG_ALERT_SUPPORT == STD_ON)
#define TLS_LOG_ALERT(txt) \
  CANOE_WRITE_STRING(txt)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_ALERT_1(txt, p1) \
  CANOE_WRITE_STRING_1(txt, p1)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_ALERT_2(txt, p1, p2) \
  CANOE_WRITE_STRING_2(txt, p1, p2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_ALERT_3(txt, p1, p2, p3) \
  CANOE_WRITE_STRING_3(txt, p1, p2, p3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_LOG_ALERT_4(txt, p1, p2, p3, p4) \
  CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define TLS_LOG_ALERT(txt)
#define TLS_LOG_ALERT_1(txt, p1)
#define TLS_LOG_ALERT_2(txt, p1, p2)
#define TLS_LOG_ALERT_3(txt, p1, p2, p3)
#define TLS_LOG_ALERT_4(txt, p1, p2, p3, p4)
#endif

#define Tls_GetRxPlainTextStartPtr(TConnId)   (Tls_VCfgGetRxPlainTextStarts()[TConnId])  /* Get the pointer to the first free byte in the plain text buffer */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define Tls_GetRxPlainWritePtr(TConnId)   (&(((uint8*)(Tls_VCfgGetRxPlainTextStarts()[TConnId]))[Tls_RxPlainTextFillLevel[TConnId]]))  /* Get the pointer to the first free byte in the plain text buffer */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Read 3 bytes out of a buffer into a uint32 variable. Implementation is similar to IPBASE_GET_UINT32(). */
#define TLS_GET_UINT24(Buffer, Offset, Variable) ( (Variable) = (uint32)(((uint32)((Buffer)[(Offset)+0]) << 16) | \
                                                                         ((uint32)((Buffer)[(Offset)+1]) <<  8) | \
                                                                         ((uint32)((Buffer)[(Offset)+2]))) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* used in Tls_CryptoCalcPending */
#define TLS_CRYPTO_CALC_IDLE                     0x00u
#define TLS_CRYPTO_CALC_RX_SERVER_CERT_CRL       0x01u
#define TLS_CRYPTO_CALC_RX_SERVER_CERT_CHAIN     0x02u
#define TLS_CRYPTO_CALC_RX_SERVER_CERT_LAST      0x03u

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef enum
{
   TLS_E_OK,
   TLS_E_NOT_OK,
   TLS_E_SMALL_BUFFER
} Tls_ReturnType;

typedef struct
{
  /* standard definitions have all values as 'int' */
        uint8 Sec;     /* seconds after the minute - [0,59] */
        uint8 Min;     /* minutes after the hour - [0,59] */
        uint8 Hour;    /* hours since midnight - [0,23] */
        uint8 Mday;    /* day of the month - [1,31] */
        uint8 Mon;     /* months since January - [0,11] */
        uint8 Year;    /* years since 1900 */
} Tls_TimeDTType;

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#define TLS_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
STATIC VAR(Tls_StateType, TLS_VAR_ZERO_INIT) Tls_State = TLS_STATE_UNINIT; /* Tls needs state also without DET */
#define TLS_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
STATIC VAR(NvM_BlockIdType, TLS_VAR_NOINIT) Tls_MainNvmRequBlockId;  /* block id of cert currently requested */
#if (TLS_SUPPORT_CRL)
STATIC VAR(NvM_BlockIdType, TLS_VAR_NOINIT) Tls_CrlMainNvmRequBlockId;  /* block id of CRL currently requested */
#endif

#define TLS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
STATIC VAR(uint32, TLS_VAR_NOINIT) Tls_MainFctCounter;
#define TLS_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#define TLS_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_MainNvmRequStatus;     /* status of cert currently requested */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_ServCertsIdxActive;    /* id of cert currently parsed (not the block id) */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_ServCertsParseStatus;  /* status of parsing all server certs */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_NvmExchStatus;         /* TLS global NVM echange buffer status */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_NvmRetryCnt;           /* TLS global NVM retry counter */  /* PRQA S 3218 */ /* MD_TLS_8.7_3218 */

#if (TLS_SUPPORT_CRL == STD_ON)
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_CrlMainNvmRequStatus;  /* status of CRL currently requested */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_CrlIdxActive;          /* id of CRL currently parsed (not the block id) */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_CrlParseStatus;        /* status of parsing all CRLs */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_CrlNvmExchStatus;      /* TLS global NVM echange buffer status for CRL */
STATIC VAR(uint8, TLS_VAR_NOINIT) Tls_CrlNvmRetryCnt;        /* TLS global NVM retry counter for CRL */  /* PRQA S 3218 */ /* MD_TLS_8.7_3218 */
#endif

#if ( TLS_VENABLE_CANOE_WRITE_STRING == STD_ON )
STATIC VAR(sint8, TLS_VAR_NOINIT) Tls_CanoeWriteStr[256];
#endif
#define TLS_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL CONST DATA
 **********************************************************************************************************************/

#define TLS_START_SEC_CONST_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
STATIC CONST(uint8, TLS_CONST) Tls_HeartbeatRequestString[] = "Heartbeat ";
#endif

/* labels and their lengths used in some handshake messages */
STATIC CONST(uint8, TLS_CONST) Tls_Hs_ClientFinishedLabel[]  = "client finished";   /* TLS_LABEL_CLIENT_FINISHED_LEN */  /* PRQA S 3218 */ /* MD_TLS_8.7_3218 */
STATIC CONST(uint8, TLS_CONST) Tls_Hs_ServerFinishedLabel[]  = "server finished";   /* TLS_LABEL_SERVER_FINISHED_LEN */  /* PRQA S 3218 */ /* MD_TLS_8.7_3218 */
STATIC CONST(uint8, TLS_CONST) Tls_Hs_MasterSecretLabel[]    = "master secret";     /* TLS_LABEL_MASTER_SECRET_LEN */
STATIC CONST(uint8, TLS_CONST) Tls_Hs_KeyExpansionLabel[]    = "key expansion";     /* TLS_LABEL_KEY_EXPENSION_LEN */    /* PRQA S 3218 */ /* MD_TLS_8.7_3218 */
#define TLS_STOP_SEC_CONST_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_CONST_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
STATIC CONST(uint16, TLS_CONST) Tls_HeartbeatRequestStringTextLen = 10;
STATIC CONST(uint16, TLS_CONST) Tls_HeartbeatCounterStringLen     = 4;
#endif
#define TLS_STOP_SEC_CONST_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define TLS_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (TLS_SUPPORT_CRL == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VMain_ValidateCertAgainstCrl(
    Tls_TConnIdType TConnId );
#endif

STATIC FUNC(void, TLS_CODE) Tls_VMain_SendPendingApplData(
    Tls_TConnIdType TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VMain_HandlePendingAlerts(
    Tls_TConnIdType TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VMain_ProcessHsDataAfterNvmAccess(
    Tls_TConnIdType TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VMain_ProcessHsStates(
    Tls_TConnIdType TConnId );

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
/***********************************************************************************************************************
 *  Tls_VMain_HeartbeatHandling
 **********************************************************************************************************************/
/*! \brief         Do the heartbeat timeout handling.
 *  \param[in]     TConnId        connection index
 *  \return        void
 *  \context       MainFunction
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VMain_HeartbeatHandling(
    Tls_TConnIdType TConnId );
#endif

STATIC FUNC(void, TLS_CODE) Tls_VParseAllStoredServCerts( void );

#if (TLS_SUPPORT_CRL == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VParseAllStoredCrls( void );
#endif

STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientHello(
    uint8 TConnId );

#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientKeyExchangeRsa(
    uint8 TConnId );
#endif

#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientKeyExchangeEcdh(
    uint8 TConnId );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_CalcEcdhMasterSecret(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) PrivKeyPtr );
#endif

STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientCert(
    uint8 TConnId );

#if (TLS_SUPPORT_CLIENT_CERT == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientCertVeri(
    uint8 TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VHs_AddRsaSignToClientCertVeriMsg(
    uint8 TConnId,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_VAR) MsgPtr,
    uint16 HashOffset,
    uint16 HashSize,
    uint16 ExtraLenHashAndSign,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_VAR) SignInputPtr,
    P2VAR(uint16, AUTOMATIC, TLS_APPL_VAR) SignLenPtr /*out*/ );

STATIC FUNC(void, TLS_CODE) Tls_VHs_AddEcdsaSignToClientCertVeriMsg(
    uint8  TConnId,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_VAR) MsgPtr,
    uint16 ExtraLenHashAndSign,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_VAR) SignInputPtr,
    P2VAR(uint16, AUTOMATIC, TLS_APPL_VAR) SignLenPtr /*out*/ );
#endif

STATIC FUNC(void, TLS_CODE) Tls_VCcs_SendChangeCipherSpec(
    uint8 TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientFinished(
    uint8 TConnId );

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VHb_SendHeartbeatRequest(
    uint8 TConnId );

STATIC FUNC(uint16, TLS_CODE) Tls_VHb_Rx(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen );
#endif

STATIC FUNC(void, TLS_CODE) Tls_VHs_HashesCopyAndUpdate(
    uint8 TConnId,
    uint16 DataLen,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr);

STATIC FUNC(void, TLS_CODE) Tls_VHs_HashesRevert(
    uint8 TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VHs_TriggerAlertAndFin(
    uint8 TConnId,
    uint8 AlertLevel,
    uint8 AlertId );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VAl_SendAlert(
    uint8 TConnId,
    uint8 AlertLevel,
    uint8 AlertId );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_Transmit(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 DataLen,
    uint8 Type );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_TransmitWithNull(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) RlHdrBufPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) MacDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 DataLen );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_TransmitWithAesCbc(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) RlHdrBufPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) MacDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 DataLen );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_TransmitWithAesGcm(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) RlHdrBufPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 DataLen,
    uint8 Type );

STATIC FUNC(uint8, TLS_CODE) Tls_VRl_RxHdr(
    uint8 TConnId,
    P2CONST(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset );

STATIC FUNC(uint16, TLS_CODE) Tls_VRl_RxPayload(
    uint8 TConnId,
    P2VAR(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset,
    uint16 RxLen );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_RxPayloadWithNull(
    uint8 TConnId,
    P2CONST(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset,
    uint16 RxLen );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_RxPayloadWithAesCbc(
    uint8 TConnId,
    P2VAR(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset,
    uint16 RxLen );

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
#if (TLS_SUPPORT_AES_GCM == STD_ON)
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_RxPayloadWithAesGcm(
    uint8 TConnId,
    P2VAR(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset,
    uint16 RxLen );

STATIC FUNC(Tls_ReturnType, TLS_CODE) Tls_VAesGcmDecryptData(
    uint8 TConnId,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_DATA) WsPtr,
    P2CONST(uint8,  AUTOMATIC, TLS_APPL_DATA) CipherTextPtr,
    uint16 CipherTextLength,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_DATA) PlainTextPtr,
    P2VAR(uint16,  AUTOMATIC, TLS_APPL_DATA) PlainTextLengthPtr,
    boolean Finished );
#endif
#endif

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_ForwardRxData(
    uint8 TConnId );

STATIC FUNC(uint16, TLS_CODE) Tls_VHs_Rx(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen,
    uint16 CurrentReadPos );

STATIC FUNC(uint16, TLS_CODE) Tls_VAl_Rx(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen );

STATIC FUNC(uint16, TLS_CODE) Tls_VAd_Rx(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen );

STATIC FUNC(uint16, TLS_CODE) Tls_VCcs_Rx(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen );

STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxMsg(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen );

STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerHello(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen );

STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerCert(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_RxServerCert_ChainCheck(
    uint8 TConnId );

#if (TLS_SUPPORT_CRL == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VHs_RxServerCert_FindMatchingCrl(
    Tls_TConnIdType TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VCrlReadCrlFromNvm(
    Tls_TConnIdType TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VCrlCheckCrlUpdateTime(
    Tls_TConnIdType TConnId );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VSearchForSerialInCrl(
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertListPtr,
    uint16 CertListLen,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SerialPtr,
    uint8 SerialLen,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) CrlSeachStatusPtr );
#endif

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_CheckCertTimeValidity(
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertStartPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertDescrPtr );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_CheckRecServCert(
    uint8 TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VHs_CheckLastCertAgainstRoot(
    uint8 TConnId );

STATIC FUNC(uint16, TLS_CODE) Tls_VHs_ReadSignAlgId(
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SignAlgPtr,
    uint8 SignAlgLen );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSign(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr );

#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
#if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSignMd5Rsa(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr );
#endif  /* (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON) */

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSignSha1Rsa(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSignXxRsa(
    uint8 TConnId,
    uint8 HashAlg,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr );
#endif

#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSignXxEcdsa(
    uint8 TConnId,
    uint8 HashAlg,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr );
#endif

#if (TLS_SUPPORT_OCSP == STD_ON)
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerCertStatus(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr );

STATIC FUNC(boolean, TLS_CODE) Tls_VOcspCheckSerialInServerCert(
    uint8 TConnId,
    P2CONST(Tls_Ocsp_RespDescrType, AUTOMATIC, TLS_CONST) StatusDescrPtr,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) ResponseMsgPtr );
#endif

STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerKeyExchange(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr );

/***********************************************************************************************************************
 *  Tls_VHs_RxCertRequest
 **********************************************************************************************************************/
/*! \brief      Receive handshake msg certificate_request
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \return                    consumed length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxCertRequest(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr );

STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerFinished(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VPrf(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SecretPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) LabelPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SeedPtr,
    uint8 TgtLen,
    uint8 SecretLen,
    uint8 LabelLen,
    uint8 SeedLen );

#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VP_Md5(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SecretPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SeedPtr,
    uint16 TgtLen,
    uint16 SecretLen,
    uint16 SeedLen );
#endif  /* (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON) */

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VP_Sha256(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SecretPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SeedPtr,
    uint16 TgtLen,
    uint16 SecretLen,
    uint16 SeedLen );
#endif  /* (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON) */

#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VP_Sha1(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SecretPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SeedPtr,
    uint16 TgtLen,
    uint16 SecretLen,
    uint16 SeedLen );
#endif  /* (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON) */

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VCalcKeyBlock(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtPtr,
    uint8 TgtSize );

STATIC FUNC(void, TLS_CODE) Tls_VSeqNumInit(
    P2VAR(Tls_SeqNumType, AUTOMATIC, TLS_APPL_DATA) SeqNumPtr );

STATIC FUNC(void, TLS_CODE) Tls_VSeqNumInc(
    P2VAR(Tls_SeqNumType, AUTOMATIC, TLS_APPL_DATA) SeqNumPtr );

STATIC FUNC(Tls_ReturnType, TLS_CODE) Tls_VAesEncryptData(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) IVPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) PlainTextPtr,
    uint16 PlainTextLength,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) CipherTextPtr,
    P2VAR(uint16, AUTOMATIC, TLS_APPL_DATA) CipherTextLengthPtr,
    boolean Finished );

STATIC FUNC(Tls_ReturnType, TLS_CODE) Tls_VAesDecryptData(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) IVPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CipherTextPtr,
    uint16 CipherTextLength,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) PlainTextPtr,
    P2VAR(uint16, AUTOMATIC, TLS_APPL_DATA) PlainTextLengthPtr,
    boolean Finished );

STATIC FUNC(void, TLS_CODE) Tls_VInitConnVars(
    uint8 TConnId );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_CheckRxMac(
    uint8 TConnId );

STATIC FUNC(void, TLS_CODE) Tls_VReadCertTimeInfo(
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint8 Len,
    CONSTP2VAR(uint32, AUTOMATIC, TLS_CONST) Time32Ptr );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VParseServerCert(
    CONSTP2VAR(Tls_Cert_Descr, AUTOMATIC, TLS_CONST) CertDesc,
    uint8 HashInfo,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen );

#if (TLS_SUPPORT_CRL == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VParseBerStruct(
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen,
    P2VAR(Tls_Cert_EleInfo, AUTOMATIC, TLS_APPL_DATA) BerStructDescPtr,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) BerChapterTablePtr,
    uint8 BerChapterTableSize,
    uint8 BerChapterTableDepth,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) BerChapterDepthPtr,
    P2VAR(uint32, AUTOMATIC, TLS_APPL_DATA) FoundElementFlagsPtr
    );

STATIC void Tls_VHashSomeDataWithSha1(
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) HashOutputPtr );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VParseCrl(
    uint8 CrlId,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen );
#endif

STATIC FUNC(uint8, TLS_CODE) Tls_VReadKeyAlgId(
    CONSTP2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen );

STATIC FUNC(void, TLS_CODE) Tls_VHs_HashRxMsg(
    uint8 TConnId,
    CONSTP2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen );

STATIC FUNC(void, TLS_CODE) Tls_VHs_HashMsg(
    uint8 TConnId,
    CONSTP2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen );

STATIC FUNC(void, TLS_CODE) Tls_VConvertTimeDT2S(
    P2CONST(Tls_TimeDTType, AUTOMATIC, TLS_APPL_DATA) TimeDTPtr,
    P2VAR(uint32, AUTOMATIC, TLS_APPL_DATA) TimeSPtr );

#if (TLS_SUPPORT_OCSP == STD_ON)
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VCopyPubKeyFromCert(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) CertPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertDescrPtr );

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VParseCertStatus(
    CONSTP2VAR(Tls_Ocsp_RespDescrType, AUTOMATIC, TLS_CONST) StatusDescr,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen );
#endif

STATIC FUNC(void,              TLS_CODE) Tls_VInitBufSegment(
                                        P2VAR(Tls_BufSegMgtType, AUTOMATIC, TLS_APPL_DATA) BufSegPtr);
STATIC FUNC(Std_ReturnType,    TLS_CODE) Tls_VWriteBufSegment(
                                        Tls_TConnIdType TConnId,
                                        P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) SrcBufPtr,
                                        uint16 SrcBufLen,
                                        P2VAR(Tls_BufSegMgtType, AUTOMATIC, TLS_APPL_DATA) TgtBufSegPtr);
STATIC FUNC(void, TLS_CODE) Tls_VGetPbufFromBufSegment(
                        P2CONST(Tls_BufSegMgtType, AUTOMATIC, TLS_APPL_DATA) BufSegPtr,
                        P2VAR(IpBase_PbufType, AUTOMATIC, TLS_APPL_VAR) PbufPtr );
STATIC FUNC(void,              TLS_CODE) Tls_VReleaseBufSegment(
                                        Tls_TConnIdType TConnId,
                                        P2VAR(Tls_BufSegMgtType, AUTOMATIC, AUTOMATIC) BufSegPtr,
                                        uint32 Len);
STATIC FUNC(void, TLS_CODE) Tls_VResetSockConnMappings(
    TcpIp_SocketIdType SocketId );

STATIC FUNC(void, TLS_CODE) Tls_VMain_ProcessReceivedData(Tls_TConnIdType TConnId);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VCP_SetRxTxBufferSize(
    Tls_SocketIdType SocketId,
    TcpIp_ParamIdType ParameterId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) ParameterValuePtr );

#if ((defined TLS_PROCESSOR_CANOEEMU) && (TLS_VENABLE_CANOE_WRITE_STRING == STD_ON))
/***********************************************************************************************************************
 *  Tls_VConvertByteArray2HexString
 **********************************************************************************************************************/
/*! \brief         Convert a byte array into a hex string and write it into the CANoe output string.
 *  \details       -
 *  \param[in]     ArrayPtr           Pointer to the byte array
 *  \param[in]     ArrayLen           Length of the byte array
 *  \return        void
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VConvertByteArray2HexString(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) ArrayPtr,
    uint8 ArrayLen );
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Tls_InitMemory
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TLS_CODE) Tls_InitMemory(void)
{
  /* #10 Uninitialize state variable. */
  Tls_State = TLS_STATE_UNINIT;
}

/***********************************************************************************************************************
 *  Tls_Init
 **********************************************************************************************************************/
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
 */
FUNC(void, TLS_CODE) Tls_Init(
    P2CONST(Tls_ConfigType, TLS_PBCFG, TLS_CONST) CfgPtr )  /* PRQA S 3206 */ /* MD_TLS_3206 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId  = TLS_E_NO_ERROR;
  uint8          tConnId;

  TLS_UNUSED_PARAMETER(CfgPtr)  /* PRQA S 3112 */ /* MD_MSR_14.2 */

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters and preconditions. */
  /* check that Crypto module is initialized */
  if (FALSE == Crypto_CheckInit())
  {
    errorId = TLS_E_CRYPTO_NOT_INIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

  /* #20 Init variables for each TLS connection. */
  for(tConnId = 0; tConnId < Tls_VCfgGetNumConnections(); tConnId++)
  {
    Tls_VInitConnVars(tConnId);

    /* Tls_Sock2ConnMapper has TLS_CFG_NUM_CONNECTIONS elements, but there is no direct mapping between the element index and the tConnId */
    Tls_Sock2ConnMapper[tConnId].SocketId = TCPIP_INV_SOCK_IDX;
    Tls_Sock2ConnMapper[tConnId].RxBufferSize = 0;
    Tls_Sock2ConnMapper[tConnId].TxBufferSize = 0;

#if (TLS_SUPPORT_CRL == STD_ON)
    Tls_CrlValidationState[tConnId] = TLS_CRL_VALIDATION_IDLE;
#endif

  }

  /* #30 Reset/invalidate all server (root) certificate descriptors. */
  {
    uint8 CertIdx;
    for(CertIdx = 0; CertIdx < Tls_VCfgGetNumServerCerts(); CertIdx++)
    {
      Tls_ServerCertDescs[CertIdx].CertWasParsed = FALSE;
    }
  }

#if (TLS_SUPPORT_CRL == STD_ON)
  /* #40 Reset/invalidate all CRL descriptors. */
  {
    uint8 CrlIdx;
    for(CrlIdx = 0; CrlIdx < TLS_CFG_NUM_CRLS; CrlIdx++)
    {
      Tls_CrlDescs[CrlIdx].CrlWasParsed = FALSE;
    }
  }
#endif

  /* #50 Init variables used for elements that are stored in NVM (root certs and CRLs). */
  Tls_ServCertsIdxActive    = TLS_UINT8_INV_VALUE;
  Tls_ServCertsParseStatus  = TLS_SERV_CERT_PARSE_NONE;
  Tls_NvmExchStatus         = TLS_NVM_ACCESS_IDLE;
  Tls_MainNvmRequBlockId    = TLS_NVM_INV_BLOCK_ID;
  Tls_MainNvmRequStatus     = TLS_NVM_REQU_IDLE;
#if (TLS_SUPPORT_CRL == STD_ON)
  Tls_CrlIdxActive          = TLS_UINT8_INV_VALUE;
  Tls_CrlParseStatus        = TLS_CRL_PARSE_NONE;
  Tls_CrlNvmExchStatus      = TLS_NVM_ACCESS_IDLE;
  Tls_CrlMainNvmRequBlockId = TLS_NVM_INV_BLOCK_ID;
  Tls_CrlMainNvmRequStatus  = TLS_NVM_REQU_IDLE;
#endif

  /* #60 Initialize the variables for the mapping between sockets and connections. */
  {
    uint8                   sockId;
    CONST(uint8, TLS_CONST) numTConns       = Tls_VCfgGetNumConnections();
    CONST(uint8, TLS_CONST) numTcpIpSockets = Tls_VCfgGetNumTcpIpSockets();

    for(sockId = 0; sockId < numTcpIpSockets; sockId++)
    {
      Tls_SockHnd2TlsConnMap[sockId] = TLS_INV_CONN_ID;
    }
    for(tConnId = 0; tConnId < numTConns; tConnId++)
    {
      Tls_CfgTlsConn2SockHndMap[tConnId] = (Tls_SocketIdType)TCPIP_INV_SOCKET_ID;
    }
  }

  /* #70 Init MainFunctionCounter and set module state to 'initialized'. */
  Tls_State          = TLS_STATE_INIT;
  Tls_MainFctCounter = 0;

  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #80 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_INIT, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_GetVersionInfo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
#if (TLS_VERSION_INFO_API == STD_ON)
FUNC(void, TLS_CODE) Tls_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, TLS_APPL_DATA) VersionInfoPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId  = TLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check parameter 'VersionInfoPtr' */
  if (TLS_COND_INVALID_PTR(VersionInfoPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Assign the module info and version. */
    VersionInfoPtr->vendorID = TLS_VENDOR_ID;
    VersionInfoPtr->moduleID = TLS_MODULE_ID;
    VersionInfoPtr->sw_major_version = TLS_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = TLS_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = TLS_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_GET_VERSION_INFO, errorId);
  }
# else
  TLS_UNUSED_PARAMETER(errorId)            /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (TLS_VERSION_INFO_API == STD_ON) */

/***********************************************************************************************************************
 *  Tls_GetNvmBlockIdForUsedRootCert
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_GetNvmBlockIdForUsedRootCert(
    Tls_SocketIdType SocketId,
    P2VAR(NvM_BlockIdType, AUTOMATIC, TLS_APPL_DATA) RootCertBlockIdPtr )
{
#if (TLS_SUPPORT_GET_NVM_BLOCK_ID_FOR_USED_ROOT_CERT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal   = E_NOT_OK;
  uint8          errorId  = TLS_E_NO_ERROR;
  uint8          tConnId;

  /* ----- Development Error Checks ------------------------------------- */
# if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  /* Check parameter 'RootCertBlockIdPtr' */
  else if (TLS_COND_INVALID_PTR(RootCertBlockIdPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    tConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Check if TLS is activated on this socket. */
    if(TLS_INV_CONN_ID == tConnId)
    {
      /* the given socket is not used for TLS -> error */
    }
    /* #30 Check if the TLS connection is fully established. */
    else if(TLS_STATE_TCP_ESTABL_TLS_ESTABL != Tls_ConnState[tConnId])
    {
      /* There is no connection established, so there can't be a valid stored NvmBlockId -> error */
    }
    else
    {
      /* #40 Get the used root cert NVM block ID. */
      *RootCertBlockIdPtr = Tls_UsedRootCertNvmBlockId[tConnId];
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_GET_NVM_BLOCK_ID_FOR_USED_ROOT_CERT, errorId);
  }
# else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;

#else
  /* The feature to get the used root certificate is deactivated, so the function has an empty implementation. */
  TLS_UNUSED_PARAMETER(SocketId)            /* PRQA S 3112 */ /* MD_MSR_14.2 */
  TLS_UNUSED_PARAMETER(RootCertBlockIdPtr)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
  return E_NOT_OK;
#endif  /* TLS_SUPPORT_GET_NVM_BLOCK_ID_FOR_USED_ROOT_CERT == STD_ON) */
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_RootCertWasModified
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_RootCertWasModified(
    NvM_BlockIdType NvmBlockId )
{
#if (TLS_SUPPORT_ROOT_CERT_UPDATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  returnValue                       = E_NOT_OK;
  uint8           errorId                           = TLS_E_NO_ERROR;
  uint8           CertIdx;

  /* ----- Development Error Checks ------------------------------------- */
# if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    TLS_LOG_DIV_1("Tls_RootCertWasModified: function was called with NvmBlockId = %d.", NvmBlockId);

    /* #20 Go through all stored root certificates to find the one with the given NvmBlockId. */
    for(CertIdx = 0; CertIdx < Tls_VCfgGetNumServerCerts(); CertIdx++)
    {
      if( NvmBlockId == Tls_VCfgGetServerCertBlockIds(CertIdx) )
      {
        /* #30 The corresponding certificate was found. Invalidate its descriptor to trigger a new parsing. */
        Tls_ServerCertDescs[CertIdx].CertWasParsed = FALSE;  /* invalidate cert descriptor */
        if((TLS_SERV_CERT_PARSE_COMPLETE == Tls_ServCertsParseStatus) || (TLS_SERV_CERT_PARSE_FAILED == Tls_ServCertsParseStatus))
        {
          /* currently all  'old' certificates are parsed (as far as possible) */
          Tls_ServCertsParseStatus = TLS_SERV_CERT_PARSE_NONE;  /* trigger parsing of the updated certificate */
        }
        else
        {
          /* parsing is already active, so no extra triggering is necessary */
        }
        returnValue = E_OK;
      }
    }

    /* if no corresponding certificate was found, returnValue still is set to E_NOT_OK. */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_ROOT_CERT_WAS_MODIFIED, errorId);
  }
# else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return returnValue;

#else
  TLS_UNUSED_PARAMETER(NvmBlockId)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
  return E_NOT_OK;
#endif  /* (TLS_SUPPORT_ROOT_CERT_UPDATE == STD_ON) */
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (TLS_SUPPORT_CRL == STD_ON)
/***********************************************************************************************************************
 *  Tls_CrlWasModified
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_CrlWasModified(
    NvM_BlockIdType NvmBlockId )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  returnValue                       = E_NOT_OK;
  uint8           errorId                           = TLS_E_NO_ERROR;
  uint8           CrlIdx;

  /* ----- Development Error Checks ------------------------------------- */
# if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    TLS_LOG_DIV_1("Tls_CrlWasModified: function was called with NvmBlockId = %d.", NvmBlockId);

    TLS_CheckDetErrorReturnValue((TLS_COND_INIT), TLS_API_ID_CRL_WAS_MODIFIED, TLS_E_NOT_INITIALIZED, E_NOT_OK)

    /* #20 Go through all stored CRLs to find the one with the given NvmBlockId. */
    for(CrlIdx = 0; CrlIdx < TLS_CFG_NUM_CRLS; CrlIdx++)
    {
      if( NvmBlockId == Tls_CfgCrlBlockIds[CrlIdx] )
      {
        /* #30 The corresponding CRL was found. Invalidate its descriptor to trigger a new parsing. */
        Tls_CrlDescs[CrlIdx].CrlWasParsed = FALSE;  /* invalidate CRL descriptor */
        if((TLS_CRL_PARSE_COMPLETE == Tls_CrlParseStatus) || (TLS_CRL_PARSE_FAILED == Tls_CrlParseStatus))
        {
          /* currently all 'old' CRL are parsed (as far as possible) */
          Tls_CrlParseStatus = TLS_CRL_PARSE_NONE;  /* trigger parsing of the updated CRL */
        }
        else
        {
          /* parsing is already active, so no extra triggering is necessary */
        }
        returnValue = E_OK;
        break;  /* leave the for-loop */
      }
    }

    /* if no corresponding CRL was found, returnValue still is set to E_NOT_OK. */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_CRL_WAS_MODIFIED, errorId);
  }
# else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return returnValue;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/***********************************************************************************************************************
 *  Tls_MainFunction
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, TLS_CODE) Tls_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 TConnId;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Do nothing if module is not initialized */
  if( TLS_STATE_UNINIT == Tls_State )
  {
    /* module is not initialized or offline, so do nothing here but just return */
  }
  else
  {
    Tls_MainFctCounter++;  /* increment MainFunction counter */

    /* #20 Handle parsing of stored server certificates. */
    if(Tls_ServCertsParseStatus < TLS_SERV_CERT_PARSE_COMPLETE)
    {
      /* Tls_ServCertsParseStatus is not TLS_SERV_CERT_PARSE_COMPLETE and not TLS_SERV_CERT_PARSE_FAILED */
      Tls_VParseAllStoredServCerts();
    }

#if (TLS_SUPPORT_CRL == STD_ON)
    /* #30 Handle parsing of stored CRLs. */
    if(Tls_CrlParseStatus < TLS_CRL_PARSE_COMPLETE)
    {
      /* Tls_CrlParseStatus is not TLS_CRL_PARSE_COMPLETE and not TLS_CRL_PARSE_FAILED */
      Tls_VParseAllStoredCrls();
    }
#endif

    /* #40 Connection specific handling. */
    for(TConnId= 0; TConnId < Tls_VCfgGetNumConnections(); TConnId++)
    {

      /* #50 Check if connection is active in any way. */
      if(Tls_HandShakeState[TConnId] != TLS_HS_STATE_INIT)
      {
        /* #60 Call subfunctions for cyclic processing. */
#if (TLS_SUPPORT_CRL == STD_ON)
        /* handle validation of received server cert against CRL  */
        Tls_VMain_ValidateCertAgainstCrl(TConnId);
#endif

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
        /* handle heartbeat timeout             */
        Tls_VMain_HeartbeatHandling(TConnId);
#endif

        /* handle sending of pending appl data  */
        Tls_VMain_SendPendingApplData(TConnId);

        /* issue sending of pending alerts here */
        Tls_VMain_HandlePendingAlerts(TConnId);

        /* handle buffered rxindications */
        /* -- upper layer processing     */
        Tls_VMain_ProcessReceivedData(TConnId);

        /* process handshake data after NVM access */
        Tls_VMain_ProcessHsDataAfterNvmAccess(TConnId);

        /* start handshake state dependent actions ---------- */
        Tls_VMain_ProcessHsStates(TConnId);

        /* issue TxConfirmation ----------------------------- */
        if(0 != Tls_TxConfRemaining[TConnId])
        {
          TcpIp_Tcp_TlsTxConfirmation(Tls_VCfgGetTlsConn2SockIdMap()[TConnId], Tls_TxConfRemaining[TConnId]);
          Tls_TxConfRemaining[TConnId] = 0;
        }
      }
    }
  }

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_MainFunctionLowPrio
 **********************************************************************************************************************/
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
 */
FUNC(void, TLS_CODE) Tls_MainFunctionLowPrio(void)
{
  Tls_TConnIdType tConnId;

  /* #10 Do nothing if module is not initialized */
  if( TLS_STATE_UNINIT == Tls_State )
  {
    /* module is not initialized or offline, so do nothing here but just return */
  }
  else
  {
    for(tConnId = 0; tConnId < TLS_CFG_NUM_CONNECTIONS; tConnId++)
    {
      /* #20 Process crypto operations for each TLS connection if pending. */
      switch(Tls_CryptoCalcPending[tConnId])
      {
      case TLS_CRYPTO_CALC_IDLE:
        {
          /* #30 Do nothing if there is no calculation pending. */
          break;
        }
      case TLS_CRYPTO_CALC_RX_SERVER_CERT_CHAIN:
        {
          /* #40 Check the certificate chain inside the 'server_certificate' message. */
          if(E_OK == Tls_VHs_RxServerCert_ChainCheck(tConnId))  /* resetting the crypto-flag is done in this function */
          {
            /* Start validation of the received server certificate against the stored root certificates */

            /* store some context data */
            Tls_NumRemServCerts[tConnId] = Tls_VCfgGetNumServerCerts();

            /* #50 Trigger looking for a matching stored certificate that issued the received one */
            if(E_OK == Tls_VHs_CheckRecServCert(tConnId))
            {
              /* A matching root certificate probably has been found. The signature is not yet checked */

              Tls_RxBlockingState[tConnId] = TLS_RX_BLOCKING_STATE_LOCKED;  /* block further TCP receiption */
#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
              Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[tConnId], TLS_E_UNKNOWN_CA, FALSE);  /*  the validation versus the root certificate is succesfully finished */
#endif

              /* go on to the next handshake element */
              Tls_Ul_CurrentElement[tConnId] = TLS_HS_S_IN_TYPE;
              TLS_LOG_HS_DIV("Tls_VHs_RxServerCert: finished reading server_certificate message");
            }
            else
            {
              /* #60 Error, no matching root certificate found, cancel handshake. */
              uint8 validationResult = TCPIP_TLS_VALIDATION_UNKNOWN_CA;
              /* report the negative validation result and forward cert chain to socket owner */
              TcpIp_Tcp_TlsValidationResult((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[tConnId], &validationResult, Tls_HsServerCertChainPtr[tConnId], Tls_HsServerCertChainLen[tConnId]);

              Tls_Ul_CurrentElement[tConnId] = TLS_HS_S_IN_TYPE;
              Tls_Rl_CurrentPart[tConnId]    = TLS_RL_IN_HDR;
              /* The TLS validation result was forwarded to the socket owner, and now the validation decision of the owner is available. */
              if(validationResult == TCPIP_TLS_VALIDATION_OK)
              {
                /* The user has overruled the Tls validation result and wants to accept the connection */
                Tls_ConnNvmRequStatus[tConnId] = TLS_NVM_REQU_IDLE;
                Tls_HandShakeState[tConnId]    = TLS_HS_STATE_CERT_REC;
                Tls_CryptoCalcPending[tConnId] = TLS_CRYPTO_CALC_IDLE;

                if(0 == Tls_HsContinueReadPos[tConnId])
                {
                  /* continue with next record layer element */
                  Tls_RxBlockingState[tConnId] = TLS_RX_BLOCKING_STATE_NONE;
                }
                else
                {
                  /* continue with next handshake element inside record layer message */
                  uint8 *PlainTextDataPtr;

                  /* received server cert is validated successfully. Now call function to procede receiving handshake messages */
                  PlainTextDataPtr = Tls_GetRxPlainTextStartPtr(tConnId);
                  (void)Tls_VHs_Rx(tConnId, PlainTextDataPtr, Tls_Rl_DecryptedDataLen[tConnId], Tls_HsContinueReadPos[tConnId]);
                }
              }
              else
              {
                /* Validation result is not 'OK', so cancel the handshake */
                Tls_VHs_TriggerAlertAndFin(tConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNKNOWN_CA);
#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
              Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[tConnId], TLS_E_UNKNOWN_CA, TRUE);  /* no matching root cert or validation failed */
#endif
              }
            }
          }
          else
          {
            /* #70 Error, there was an error while checking the received chain, so cancel the handshake. */

            /* cancel handshake */
            Tls_VHs_TriggerAlertAndFin(tConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INT_ERROR);  /* error, cancel handling for this TLS connection, go on with the next TLS connection */
          }

          /* reset the crypto-flag */
          Tls_CryptoCalcPending[tConnId] = TLS_CRYPTO_CALC_IDLE;
          break;
        }
      case TLS_CRYPTO_CALC_RX_SERVER_CERT_LAST:
        {
          /* #80 Verify the server cert signature */
          Tls_VHs_CheckLastCertAgainstRoot(tConnId);
          break;
        }
      default:
        {
          /* invalid value, this can never happen since there are no more flag values */
        }
      }  /* switch */
    }  /* for(tConnId) */
  }  /* if(INIT) */
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VMain_ProcessReceivedData
 **********************************************************************************************************************/
/*! \brief         Processing of received data.
 *  \details       -
 *  \context       MainFunction
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VMain_ProcessReceivedData(Tls_TConnIdType TConnId)
{
  /* check if there is some received data available */
  if((Tls_RxBufSegMgt[TConnId].Lvl != 0) && (Tls_CryptoCalcPending[TConnId] == TLS_CRYPTO_CALC_IDLE) && (TLS_RX_BLOCKING_STATE_NONE == Tls_RxBlockingState[TConnId]))
  {
    uint16 GlobReadPos = 0;  /* global read position inside the pbuf */
    uint16 GlobRemLen;
    IpBase_PbufType *PbufPtr = &Tls_RxPbufList[TConnId *2];

    while((GlobReadPos < PbufPtr->totLen) && (Tls_RxBlockingState[TConnId] == TLS_RX_BLOCKING_STATE_NONE))
    {
      GlobRemLen = (uint16)PbufPtr->totLen - GlobReadPos;

      /* Depending on the part of the handled record layer message (header or body), work on this part
      - header -> analyze record header
      - body   -> analyze messages */
      if(TLS_RL_IN_HDR == Tls_Rl_CurrentPart[TConnId])
      {
        /* currently receiving record layer header */
        /* the record layer header is NEVER encrypted but is plain text */
        uint8 ConsumedLen;

        ConsumedLen = Tls_VRl_RxHdr(TConnId, PbufPtr, GlobReadPos);
        if(0 != ConsumedLen)
        {
          if(TLS_RL_IN_DATA == Tls_Rl_CurrentPart[TConnId])
          {
            /* The whole RL header has been received now, and the RL header has ben removed from the TCP buffer. So now reset the read position to 0. */
            GlobReadPos = (uint16)(GlobReadPos + ConsumedLen - TLS_RL_HDR_LEN);
          }
          else
          {
            GlobReadPos += ConsumedLen;
          }
        }
        else
        {
          /* error while receiving header */
          /* send alert and close connection */
          Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
          Tls_Rl_CurrentPart[TConnId] = TLS_RL_IN_IGNORE;
          break;
        }

      }
      else if(TLS_RL_IN_DATA == Tls_Rl_CurrentPart[TConnId])
      {
        /* currently receiving record layer data */
        /* message will be decrypted inside next function (Tls_VRl_RxPayload) */

        if( GlobRemLen > Tls_Rl_RemPayloadLen[TConnId] )
        {
          /* there is another TLS msg (including a new RL header) in the indicated data
          -> only evaluate msg until the new one beginns -> len = Tls_Rl_RemPayloadLen */
          uint16 ConsumedLen;
          /* call function to decrypt and consume payload */
          ConsumedLen = Tls_VRl_RxPayload(TConnId, PbufPtr, GlobReadPos, Tls_Rl_RemPayloadLen[TConnId]);
          if(0 != ConsumedLen)
          {
            if(TLS_RX_BLOCKING_STATE_LOCKED == Tls_RxBlockingState[TConnId])
            {
              Tls_RxRemainingAckLen[TConnId] = (uint16)PbufPtr->totLen;
            }
            GlobReadPos += ConsumedLen;
          }
        }
        else
        {
          /* evaluate the whole rest of the msg */
          uint16 ConsumedLen;
          /* call function to decrypt payload */
          ConsumedLen = Tls_VRl_RxPayload(TConnId, PbufPtr, GlobReadPos, ((uint16)PbufPtr->totLen - GlobReadPos));
          if(0 != ConsumedLen)
          {
            GlobReadPos += ConsumedLen;
            if(TLS_RX_BLOCKING_STATE_LOCKED == Tls_RxBlockingState[TConnId])
            {
              Tls_RxRemainingAckLen[TConnId] = (uint16)PbufPtr->totLen;
            }
          }
          else
          {
            /* error */
            break;  /* leave the for-loop */  /* PRQA S 0771 */ /* MD_TLS_TCP_14.6_0771_a */
          }
        }

        if(0 == Tls_Rl_RemPayloadLen[TConnId])
        {
          /* record layer message has ended, start with next record layer header */
          TLS_LOG_RX_IND("Tls_RxIndication: ----- End of RL data");
          Tls_Rl_CurrentPart[TConnId] = TLS_RL_IN_HDR;
          GlobReadPos                 = 0;  /* RL element has been removed from pbuf */
        }

      }
      else if(TLS_RL_IN_IGNORE == Tls_Rl_CurrentPart[TConnId])
      {
        /* there might be some more packets received after triggering of closing (message life time in a network)
        -> ignore these packets */
        TLS_LOG_RX_IND("Tls_RxIndication: message received after triggering a fatal error, message is ignored");
        break;  /* This is a real break or error condition */  /* PRQA S 0771 */ /* MD_TLS_TCP_14.6_0771_a */
      }
      else
      {
        /* error, invalid message part, this should never happen */
        TLS_LOG_RX_IND("Tls_RxIndication: ERROR, invalid RL message part detected");
        break;  /* This is a real break or error condition */  /* PRQA S 0771 */ /* MD_TLS_TCP_14.6_0771_a */
      }
    }  /* end of while loop */
  }
}  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (TLS_SUPPORT_CRL == STD_ON)
/***********************************************************************************************************************
 *  Tls_VMain_ValidateCertAgainstCrl
 **********************************************************************************************************************/
/*! \brief         Validate the received server certificate against a stored CRL.
 *  \param[in]     TConnId        connection index
 *  \return        void
 *  \context       MainFunction
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VMain_ValidateCertAgainstCrl(
    Tls_TConnIdType TConnId )
{
  switch(Tls_CrlValidationState[TConnId])
  {
  case TLS_CRL_VALIDATION_IDLE:
    {
      /* nothing to do */
      break;
    }
  case TLS_CRL_VALIDATION_CHOOSE_CRL:
    {
      /* find matching stored CRL */
      Tls_VHs_RxServerCert_FindMatchingCrl(TConnId);
      break;
    }
  case TLS_CRL_VALIDATION_GET_CRL_FROM_NVM:
    {
      /* trigger reading the CRL from the NVM */
      Tls_VCrlReadCrlFromNvm(TConnId);
      break;
    }
  case TLS_CRL_VALIDATION_CHECK_UPDATE_TIME_AND_SN:
    {
      /* check update time and look for sn in CRL */
      Tls_VCrlCheckCrlUpdateTime(TConnId);
      break;
    }
  case TLS_CRL_VALIDATION_DONE:
    {
      /* CRL check finished. Go ahead with server cert (chain) check */
      /* trigger checking the certificate (chain) -> set crypto flag */
      Tls_CryptoCalcPending[TConnId] = TLS_CRYPTO_CALC_RX_SERVER_CERT_CHAIN;  /* in case of CRL and a received certificate chain, now also the signature of the CRL shall be checked */

      /* reset validation state */
      Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_IDLE;
      break;
    }
  case TLS_CRL_VALIDATION_FAILED:
    {
      /* CRL check finally failed */
      /* reset validation state */
      Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_IDLE;

      /* cancel handshake */
      Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_CERT_REVOKED);  /* error, cancel handling for this TLS connection, go on with the next TLS connection */
      break;
    }
  default:
    {
    }
  }
}
#endif

/***********************************************************************************************************************
 *  Tls_VMain_SendPendingApplData
 **********************************************************************************************************************/
/*! \brief         Send pending application data if available.
 *  \param[in]     TConnId        connection index
 *  \return        void
 *  \context       MainFunction
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VMain_SendPendingApplData(
    Tls_TConnIdType TConnId )
{
  if(0 != Tls_TxBufRequested[TConnId])
  {
    /* forward msg to TLS record layer (L1). */
    if(E_OK == Tls_VRl_Transmit(TConnId, Tls_TxPbufList[TConnId].payload, (uint16)Tls_TxPbufList[TConnId].totLen,
      TLS_REC_CONTENT_AD))
    {
      TLS_LOG_DIV("Tls_VMain_SendPendingApplData: application data was sent");
      Tls_TxBufRequested[TConnId] = 0; /* free the TLS TxBuffer */

      /* add length to pending TxConfirmations */
      Tls_TxConfRemaining[TConnId] += (uint16)Tls_TxPbufList[TConnId].totLen;
    }
    else
    {
      /* try again in next cycle */
    }
  }
}

/***********************************************************************************************************************
 *  Tls_VMain_HandlePendingAlerts
 **********************************************************************************************************************/
/*! \brief         Send pending alerts if available.
 *  \param[in]     TConnId        connection index
 *  \return        void
 *  \context       MainFunction
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VMain_HandlePendingAlerts(
    Tls_TConnIdType TConnId )
{
  if(TLS_ALERT_DESC_NONE != Tls_PendingAlertId[TConnId])
  {
    /* Tls_ConnState[] should be TLS_CONN_STATUS_CONNECTED */
    /* send alert */
    if(E_OK == Tls_VAl_SendAlert(TConnId, Tls_PendingAlertLevel[TConnId], Tls_PendingAlertId[TConnId]))
    {
      if(E_OK == Tls_TcpClose(Tls_VCfgGetTlsConn2SockIdMap()[TConnId]))
      {
        Tls_ConnState[TConnId]      = TLS_STATE_TCP_CLOSING_TLS_CLOSED;
        Tls_PendingAlertId[TConnId] = TLS_ALERT_DESC_NONE;
      }
      else
      {
        Tls_ConnState[TConnId]      = TLS_STATE_TCP_ESTABL_TLS_CLOSING;
        Tls_PendingAlertId[TConnId] = TLS_ALERT_DESC_TCP_FIN;
      }
      TLS_LOG_DIV_1("Tls_VMain_HandlePendingAlerts: connection state changed to %d", Tls_ConnState[TConnId]);
    }
  }
  if(TLS_ALERT_DESC_TCP_FIN == Tls_PendingAlertId[TConnId])
  {
    if(E_OK == Tls_TcpClose(Tls_VCfgGetTlsConn2SockIdMap()[TConnId]))
    {
      Tls_ConnState[TConnId]      = TLS_STATE_TCP_CLOSING_TLS_CLOSED;
      Tls_PendingAlertId[TConnId] = TLS_ALERT_DESC_NONE;
      TLS_LOG_DIV_1("Tls_VMain_HandlePendingAlerts: connection state changed to %d", Tls_ConnState[TConnId]);
    }
  }
}

/***********************************************************************************************************************
 *  Tls_VMain_ProcessHsDataAfterNvmAccess
 **********************************************************************************************************************/
/*! \brief         Process handshake data after NVM access.
 *  \param[in]     TConnId        connection index
 *  \return        void
 *  \context       MainFunction
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VMain_ProcessHsDataAfterNvmAccess(
    Tls_TConnIdType TConnId )
{
  if(Tls_CryptoCalcPending[TConnId] == TLS_CRYPTO_CALC_IDLE)
  {
    if(TLS_RX_BLOCKING_STATE_LOCKED == Tls_RxBlockingState[TConnId])
    {
      if(TLS_HS_STATE_HELLO_REC == Tls_HandShakeState[TConnId])
      {
        /* currently the message "server_certificate" is being processed */
        if(TLS_NVM_REQU_PEND == Tls_ConnNvmRequStatus[TConnId])
        {
          uint8 Res;
          /* check NVM error status */
          (void)NvM_GetErrorStatus(Tls_ConnNvmRequBlockId[TConnId], &Res);  /* return value is always OK if input parameters are valid */
          if( (Res == NVM_REQ_OK) || (Res == NVM_REQ_NV_INVALIDATED) )
          {
            /* block is read from NVM, so it can be checked now */
            Tls_ConnNvmRequStatus[TConnId] = TLS_NVM_REQU_READY;
            Tls_RxBlockingState[TConnId] = TLS_RX_BLOCKING_STATE_UNLOCKING;

            if(E_OK != Tls_VHs_CheckRecServCert(TConnId))
            {
              /* error validating the received certificate */
              /* trigger sending of alert and set TLS state to closing */
              Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_BAD_CERT);
            }
          }
        }
        else if(TLS_NVM_REQU_BUSY == Tls_ConnNvmRequStatus[TConnId])
        {
          uint8 NvMReadState;
          /* get the status of the NvM block */
          (void)NvM_GetErrorStatus(Tls_ConnNvmRequBlockId[TConnId], &NvMReadState);  /* return value is always OK if input parameters are valid */
          /* check if the block is not busy */
          if ( NVM_REQ_PENDING != NvMReadState )
          {
            /* start to read the certificate and check if the read command was successful */
            if ( E_OK == NvM_ReadBlock(Tls_ConnNvmRequBlockId[TConnId], &Tls_NvmExchData[0]) )
            {
              /* go to pending state */
              Tls_ConnNvmRequStatus[TConnId] = TLS_NVM_REQU_PEND;
            }
          }
        }
        else
        {
          /* invalid state, should not happen */
        }
      }
    }
    else
    {
      /* nothing to do */
    }
  }

}  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VMain_ProcessHsStates
 **********************************************************************************************************************/
/*! \brief         Process handshake state dependent actions.
 *  \param[in]     TConnId        connection index
 *  \return        void
 *  \context       MainFunction
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VMain_ProcessHsStates(
    Tls_TConnIdType TConnId )
{
  if(TLS_HS_STATE_HELLO_REQUESTED == Tls_HandShakeState[TConnId])
  {
    /* send CLIENT_HELLO */
    Tls_VHs_SendClientHello(TConnId);
  }

  if(TLS_HS_STATE_HELLO_DONE_REC == Tls_HandShakeState[TConnId])
  {
    if(FALSE == Tls_ClientCertRequested[TConnId])
    {
      /* send CLIENT_KEY_EXCHANGE */
      switch(Tls_Hs_KeyExchType[TConnId])
      {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
      case TLS_KEY_EXCH_TYPE_RSA:
        {
          Tls_VHs_SendClientKeyExchangeRsa(TConnId);
          break;
        }
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
      case TLS_KEY_EXCH_TYPE_ECDH:
        {
          Tls_VHs_SendClientKeyExchangeEcdh(TConnId);
          break;
        }
#endif
      default:
        {
          /* error, unsupported type -> do nothing */
          break;
        }
      }
    }
    else
    {
      /* send CLIENT_CERTIFICATE */
      Tls_VHs_SendClientCert(TConnId);
    }
  }

  if(TLS_HS_STATE_CERT_SENT == Tls_HandShakeState[TConnId])
  {
    /* send CLIENT_KEY_EXCHANGE */
    switch(Tls_Hs_KeyExchType[TConnId])
    {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
    case TLS_KEY_EXCH_TYPE_RSA:
      {
        Tls_VHs_SendClientKeyExchangeRsa(TConnId);
        break;
      }
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
    case TLS_KEY_EXCH_TYPE_ECDH:
      {
        Tls_VHs_SendClientKeyExchangeEcdh(TConnId);
        break;
      }
#endif
    default:
      {
        /* error, unsupported type -> do nothing */
        break;
      }
    }
  }

  if(TLS_HS_STATE_CL_KEY_EXCH_SENT == Tls_HandShakeState[TConnId])
  {
#if (TLS_SUPPORT_CLIENT_CERT == STD_ON)
    if( (TRUE == Tls_ClientCertRequested[TConnId]) && (0 != Tls_ClientCertLen[TConnId]) )
    {
      /* send CLIENT_CERTIFICATE_VERIFICATION */
      Tls_VHs_SendClientCertVeri(TConnId);
    }
    else
#endif
    {
      /* send CLIENT_CHANGE_CIPHER_SPEC */
      Tls_VCcs_SendChangeCipherSpec(TConnId);
    }
  }

#if (TLS_SUPPORT_CLIENT_CERT == STD_ON)
  if(TLS_HS_STATE_CERT_VERI_SENT == Tls_HandShakeState[TConnId])
  {
    /* send CLIENT_CHANGE_CIPHER_SPEC */
    Tls_VCcs_SendChangeCipherSpec(TConnId);
  }
#endif

  if(TLS_HS_STATE_CCS_SENT == Tls_HandShakeState[TConnId])
  {
    /* send CLIENT_FINISHED */
    Tls_VHs_SendClientFinished(TConnId);
  }

  if( (TLS_HS_STATE_FINISHED_REC == Tls_HandShakeState[TConnId]) &&
    (TLS_STATE_TCP_ESTABL_TLS_OPENING == Tls_ConnState[TConnId]) )
  {
    /* set connection state to connected: */
    Tls_ConnState[TConnId] = TLS_STATE_TCP_ESTABL_TLS_ESTABL;
    TLS_LOG_DIV_1("Tls_MainFct: connection state changed to %d (3 == TLS_STATE_TCP_ESTABL_TLS_ESTABL)", Tls_ConnState[TConnId]);
    Tls_HandShakeState[TConnId] = TLS_HS_STATE_CONN_ESTABLISHED;

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
    Tls_HeartbeatIdleCountdown[TConnId] = Tls_HeartbeatIdlePeriodS[TConnId] * (1000 / TLS_CFG_MAIN_FKT_PERIOD_MSEC);
#endif

    /* trigger connected callback */
    TcpIp_Tcp_TlsConnected( Tls_VCfgGetTlsConn2SockIdMap()[TConnId] );
#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
    /* TLS handshake is succesfully finished, the server did not terminate the TLS hanshake with an error alert while the TLS handshake was ongoing */
    Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[TConnId], TLS_E_SERVER_TERMINATED, FALSE);
#endif
  }
}  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
/***********************************************************************************************************************
 *  Tls_VMain_HeartbeatHandling
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_VMain_HeartbeatHandling(
    Tls_TConnIdType TConnId )
{
  /* #10 Check if heartbeat is active. */
  if( (Tls_HandShakeState[TConnId] == TLS_HS_STATE_CONN_ESTABLISHED) && (Tls_HeartbeatModeOwn[TConnId] != TLS_HB_MODE_NONE) )
  {
    Tls_HeartbeatIdleCountdown[TConnId]--;
    /* #20 Check if next heartbeat message has to be triggered. */
    if(Tls_HeartbeatIdleCountdown[TConnId] == 0)
    {
      /* #30 Trigger heartbeat timeout event if no response has been received. */
      if(Tls_HeartbeatStatus[TConnId] == TLS_HEARTBEAT_STATUS_WAIT_FOR_RESPONSE)
      {
        /* a heartbeat timeout occurred, call the user event */
        TcpIp_Tcp_TlsEvent(Tls_VCfgGetTlsConn2SockIdMap()[TConnId], TCPIP_TLS_HEARTBEAT_NO_RESPONSE);
      }

      /* #40 Send new heartbeat message and reset timers. */
      /* trigger sending the next heartbeat request */
      Tls_VHb_SendHeartbeatRequest(TConnId);

      /* reset heartbeat timer */
      Tls_HeartbeatIdleCountdown[TConnId] = Tls_HeartbeatIdlePeriodS[TConnId] * (1000 / TLS_CFG_MAIN_FKT_PERIOD_MSEC);
      Tls_HeartbeatStatus[TConnId] = TLS_HEARTBEAT_STATUS_WAIT_FOR_RESPONSE;
    }
  }
}
#endif

/***********************************************************************************************************************
 *  Tls_SetClientCertInfo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_SetClientCertInfo(
    Tls_SocketIdType SocketId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) CertPtr,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) KeyPtr,
    uint16 CertLen,
    uint16 KeyLen )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  returnValue                       = E_NOT_OK;
  uint8           errorId                           = TLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  /* Check parameter 'CertPtr' */
  else if (TLS_COND_INVALID_PTR(CertPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  /* Check parameter 'KeyPtr' */
  else if (TLS_COND_INVALID_PTR(KeyPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    uint8 TConnId;
    TConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Set cert info if possible. */
    if(Tls_ConnState[TConnId] == TLS_STATE_TCP_CLOSED_TLS_CLOSED)
    {
      Tls_ClientCertPtr[TConnId] = CertPtr;
      Tls_ClientKeyPtr[TConnId]  = KeyPtr;
      Tls_ClientCertLen[TConnId] = CertLen;
      Tls_ClientKeyLen[TConnId]  = KeyLen;
      returnValue                = E_OK;
    }
    else
    {
      /* error, nothing to do */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_SET_CLIENT_CERT_INFO, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return returnValue;

}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_GetSocket
 **********************************************************************************************************************/
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
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_GetSocket(
    TcpIp_SocketIdType SocketId )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  returnValue                       = E_NOT_OK;
  uint8           errorId                           = TLS_E_NO_ERROR;
  Tls_TConnIdType connectionMappingIndex;
  Tls_TConnIdType freeConnectionMappingElementIndex = TLS_INV_CONN_ID;
  boolean         socketAlreadyIsMappedToConnection = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #20 Iterate through all TLS connections to find a free connection. */
    for(connectionMappingIndex = 0; connectionMappingIndex < TLS_CFG_NUM_CONNECTIONS; connectionMappingIndex++)
    {
      /* #30 Check if the connection is free. */
      if(TCPIP_INV_SOCK_IDX == Tls_Sock2ConnMapper[connectionMappingIndex].SocketId)
      {
        /* A free mapper element found */
        /* #40 Check if this is the first free connection. */
        if(freeConnectionMappingElementIndex == TLS_INV_CONN_ID)
        {
          /* this is the first free element, so mark it for later usage */
          freeConnectionMappingElementIndex = connectionMappingIndex;
        }
        else
        {
          /* there was already another element marked as free, just skip this element and go ahead */
        }
      }
      /* #50 Check if the SocketId is already used for another TLS connections. */
      else if(SocketId == Tls_Sock2ConnMapper[connectionMappingIndex].SocketId)
      {
        /* Error, there is already an element for this socket, Each socket can only ues one TLS connection. */
        socketAlreadyIsMappedToConnection = TRUE;
        break;
      }
      else
      {
        /* mapping element is already used for a different socket */
      }
    }

    /* #60 Check if connection mapping was found for the socket. */
    if(socketAlreadyIsMappedToConnection == TRUE)
    {
      /* error, there is already an element for this socket */
      /* -> return E_NOT_OK */
    }
    else
    {
      /* at least one free element was found. Take the marked (first) one. */
      Tls_Sock2ConnMapper[freeConnectionMappingElementIndex].SocketId = SocketId;
      returnValue = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #70 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_GET_SOCKET, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return returnValue;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_ChangeParameter
 **********************************************************************************************************************/
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
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_ChangeParameter(
    Tls_SocketIdType SocketId,
    TcpIp_ParamIdType ParameterId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) ParameterValuePtr )  /* PRQA S 3673 */ /* MD_TLS_16.7_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TLS_E_NO_ERROR;
  uint8          tConnId;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  /* Check parameter 'ParameterValuePtr' */
  else if (TLS_COND_INVALID_PTR(ParameterValuePtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #20 Change parameter values for a socket / connection depending on the parameter id. */
    switch ( ParameterId )
    {
    case TCPIP_PARAMID_V_TLS_TXBUFSIZE:
    case TCPIP_PARAMID_V_TLS_RXBUFSIZE:
      /* #30 Set the rx or tx buffer size. */
      {
        retVal = Tls_VCP_SetRxTxBufferSize( SocketId, ParameterId, ParameterValuePtr );
      }
      break;
#if (TLS_SUPPORT_OCSP == STD_ON)
    case TCPIP_PARAMID_V_TLS_SELECT_OCSP_REQEST:
      /* #40 Set the active OCSP operation mode. */
      {
        uint8 ocspMode;

        ocspMode = *ParameterValuePtr;
        tConnId  = Tls_SockHnd2TlsConnMap[SocketId];

        if(Tls_ConnState[tConnId] != TLS_STATE_TCP_CLOSED_TLS_CLOSED)
        {
          /* Error. OCSP support may only be changed if connection is closed. Variable 'retVal' is already set. */
        }
        else
        {
          if(ocspMode <= TLS_OCSP_MODE_SUPPORT_V1_V2)
          {
            /* OCSP mode is TLS_OCSP_MODE_OFF, TLS_OCSP_MODE_SUPPORT_V1, TLS_OCSP_MODE_SUPPORT_V2 or TLS_OCSP_MODE_SUPPORT_V1_V2 */
            Tls_OcspWorkingMode[tConnId] = ocspMode;
            retVal = E_OK;
          }
          else
          {
            /* Error, invalid OCSP mode. This should not happen for valid socket owner implementations. Variable 'retVal' is already set. */
          }
        }
        break;
      }
#endif
#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
    case TCPIP_PARAMID_V_TLS_HEARTBEAT_MODE:
      /* #50 Set the active Heartbeat operation mode. */
      {
        uint8 hearbeatMode;

        hearbeatMode = *ParameterValuePtr;
        tConnId      =  Tls_SockHnd2TlsConnMap[SocketId];
        if((hearbeatMode == TLS_HB_PEER_NOT_ALLOWED_TO_SEND) || (hearbeatMode == TLS_HB_MODE_NONE))
        {
          Tls_HeartbeatModeOwn[tConnId] = hearbeatMode;
          retVal = E_OK;
        }
        else
        {
          /* Error, invalid or not supported heartbeat mode.  Variable 'retVal' is already set. */
        }
        break;
      }
    case TCPIP_PARAMID_V_TLS_HEARTBEAT_PERIOD_S:
      /* #60 Set the Heartbeat period. */
      {
        tConnId = Tls_SockHnd2TlsConnMap[SocketId];
        Tls_HeartbeatIdlePeriodS[tConnId] = *((uint16*)(ParameterValuePtr));  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_parameter */  /* PRQA S 3305 */ /* MD_TLS_3305_param */
        retVal = E_OK;
        break;
      }
#endif
    default:
      {
        /* invalid parameter id, do nothing */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #70 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_CHANGE_PARAMETER, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
}  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VCP_SetRxTxBufferSize
 **********************************************************************************************************************/
/*! \brief         Assign Rx- or Tx-Buffer to TLS connection.<br>
 *  \description   This function shall be called by Tls_ChangeParameter().
 *  \context       task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VCP_SetRxTxBufferSize(
    Tls_SocketIdType SocketId,
    TcpIp_ParamIdType ParameterId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) ParameterValuePtr )
{
  Std_ReturnType RetVal;

  /* parameter size is 32bit */
  /* Check if SocketId is used for one of the TLS connections. During Tls_GetSocket one element has already been reserved. */
  Tls_TConnIdType ConnectionCnt;
  for(ConnectionCnt = 0; ConnectionCnt < TLS_CFG_NUM_CONNECTIONS; ConnectionCnt++)
  {
    if(SocketId == Tls_Sock2ConnMapper[ConnectionCnt].SocketId)
    {
      /* mapper element found */
      break;
    }
  }

  if(ConnectionCnt < TLS_CFG_NUM_CONNECTIONS)
  {
    /* a matching mapper element was found */
    uint32 NewBufferSize = *((const uint32*)(ParameterValuePtr));  /* PRQA S 3305 */ /* MD_TLS_3305_param */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_parameter */
    Tls_TConnIdType TConnId;

    if((Tls_Sock2ConnMapper[ConnectionCnt].RxBufferSize == 0) && (Tls_Sock2ConnMapper[ConnectionCnt].TxBufferSize == 0))
    {
      /* no rx and tx buffer has already been chosen, so this is the first buffer element to be attached to the connection */
      /* go through all TLS connections to find a matching rx or tx buffer */
      for ( TConnId = 0; TConnId < TLS_CFG_NUM_CONNECTIONS; TConnId++ )
      {
        if(ParameterId == TCPIP_PARAMID_V_TLS_TXBUFSIZE)
        {
          /* find the TLS connection that has the same Tx buffer size as requested */
          if((NewBufferSize == Tls_VCfgGetTxPlainTextSizes()[TConnId]) && (Tls_CfgTlsConn2SockHndMap[TConnId] == TCPIP_INV_SOCKET_ID))
          {
            /* A matching buffer was found. The buffer will not yet be reserved. */
            Tls_Sock2ConnMapper[ConnectionCnt].TxBufferSize = NewBufferSize;
            break;  /* break the foor-loop */  /* PRQA S 0771 */ /* MD_TLS_TCP_14.6_0771_b */
          }
        }
        else  /* TCPIP_PARAMID_V_TLS_RXBUFSIZE */
        {
          /* find the TLS connection that has the same Tx buffer size as requested */
          if((NewBufferSize == Tls_VCfgGetRxPlainTextSizes()[TConnId]) && (Tls_CfgTlsConn2SockHndMap[TConnId] == TCPIP_INV_SOCKET_ID))
          {
            /* A matching buffer was found. The buffer will not yet be reserved. */
            Tls_Sock2ConnMapper[ConnectionCnt].RxBufferSize = NewBufferSize;
            break;  /* break the foor-loop */  /* PRQA S 0771 */ /* MD_TLS_TCP_14.6_0771_b */
          }
        }
      }
      if(TConnId < TLS_CFG_NUM_CONNECTIONS)
      {
        RetVal = E_OK;
      }
      else
      {
        RetVal = E_NOT_OK;  /* no matching buffer was found */
      }
    }
    else if(!((Tls_Sock2ConnMapper[ConnectionCnt].RxBufferSize != 0) && (Tls_Sock2ConnMapper[ConnectionCnt].TxBufferSize != 0)))
    {
      /* exactly one of both buffers (rx or tx) is already chosen */
      /* go through all TLS connections to find a matching buffer pair */
      for ( TConnId = 0; TConnId < TLS_CFG_NUM_CONNECTIONS; TConnId++ )
      {
        if(ParameterId == TCPIP_PARAMID_V_TLS_TXBUFSIZE)
        {
          /* find the TLS connection that has the same Tx buffer size as requested */
          if( (Tls_CfgTlsConn2SockHndMap[TConnId] == TCPIP_INV_SOCKET_ID) &&
            (NewBufferSize == Tls_VCfgGetTxPlainTextSizes()[TConnId]) &&
            (Tls_Sock2ConnMapper[ConnectionCnt].RxBufferSize == Tls_VCfgGetRxPlainTextSizes()[TConnId]) )
          {
            /* A matching buffer pair was found. The buffer will now be reserved. */
            Tls_Sock2ConnMapper[ConnectionCnt].TxBufferSize = NewBufferSize;
            Tls_Sock2ConnMapper[ConnectionCnt].TlsConnectionId = TConnId;
            Tls_CfgTlsConn2SockHndMap[TConnId]                 = SocketId;
            Tls_SockHnd2TlsConnMap[SocketId]                   = TConnId;
            break;  /* break the foor-loop */  /* PRQA S 0771 */ /* MD_TLS_TCP_14.6_0771_b */
          }
        }
        else  /* TCPIP_PARAMID_V_TLS_RXBUFSIZE */
        {
          /* find the TLS connection that has the same Tx buffer size as requested */
          if( (Tls_CfgTlsConn2SockHndMap[TConnId] == TCPIP_INV_SOCKET_ID) &&
            (NewBufferSize == Tls_VCfgGetRxPlainTextSizes()[TConnId]) &&
            (Tls_Sock2ConnMapper[ConnectionCnt].TxBufferSize == Tls_VCfgGetTxPlainTextSizes()[TConnId]) )
          {
            /* A matching buffer pair was found. The buffer will now be reserved. */
            Tls_Sock2ConnMapper[ConnectionCnt].RxBufferSize    = NewBufferSize;
            Tls_Sock2ConnMapper[ConnectionCnt].TlsConnectionId = TConnId;
            Tls_CfgTlsConn2SockHndMap[TConnId]                 = SocketId;
            Tls_SockHnd2TlsConnMap[SocketId]                   = TConnId;
            break;  /* break the foor-loop */  /* PRQA S 0771 */ /* MD_TLS_TCP_14.6_0771_b */
          }
        }
      }
      if(TConnId < TLS_CFG_NUM_CONNECTIONS)
      {
        RetVal = E_OK;
      }
      else
      {
        RetVal = E_NOT_OK;  /* no matching buffer was found */
      }
    }
    else
    {
      /* both buffers (rx and tx) are already chosen, no further changes are allowed */
      RetVal = E_NOT_OK;
    }
  }
  else
  {
    /* error, no matching mapper element was found. This should never happen if the socket was previously requested. */
    RetVal = E_NOT_OK;
  }

  return RetVal;
}  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_Close
 **********************************************************************************************************************/
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
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_Close(
    TcpIp_SocketIdType SocketId,
    boolean Abort )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TLS_E_NO_ERROR;
  uint8          tConnId;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    tConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Perform closing action dependent on the current connection state. */
    switch(Tls_ConnState[tConnId])
    {
    case TLS_STATE_TCP_OPENING_TLS_CLOSED:
      {
        /* TLS has not yet started */
        /* #30 Close the partly established TCP connection. */
        Tls_ConnState[tConnId] = TLS_STATE_TCP_CLOSED_TLS_CLOSED;
        TLS_LOG_HS_DIV_1("Tls_Close: connection state changed to %d", Tls_ConnState[tConnId]);
        if(Abort == TRUE)
        {
          Tls_VResetSockConnMappings(SocketId);
        }
        break;
      }
    case(TLS_STATE_TCP_ESTABL_TLS_OPENING):
      {
        /* TLS was about to establish a connection and is about to stop */
        /* #40 Close the partly established TLS connection. */
        /* cancel TLS connection establishment */
        Tls_ConnState[tConnId] = TLS_STATE_TCP_CLOSED_TLS_CLOSED;
        TLS_LOG_HS_DIV_1("Tls_Close: connection state changed to %d", Tls_ConnState[tConnId]);

        /* cancel handshake */
        /* reset the conn2sock & sock2conn mapping */
        if(Abort == TRUE)
        {
          Tls_VResetSockConnMappings(SocketId);
        }
        break;
      }
    case(TLS_STATE_TCP_ESTABL_TLS_ESTABL):
    case(TLS_STATE_TCP_ESTABL_TLS_CLOSING):
      {
        /* TCP connection is fully established */
        /* #50 Close the connection while the TCP connection is fully established. */

        if(Abort == FALSE)
        {
          /* issue sending of TLS_ALERT_DESC_CLOSE_NOTIFY, and close */
          Tls_VHs_TriggerAlertAndFin(tConnId, TLS_ALERT_LEVEL_WARNING, TLS_ALERT_DESC_CLOSE_NOTIFY);
        }
        else
        {
          /* TLS does not have to trigger any further action, only all variables can be reset */
        }
        TLS_LOG_HS_DIV_1("Tls_Close: connection state changed to %d", Tls_ConnState[tConnId]);
        break;
      }
    case(TLS_STATE_TCP_CLOSING_TLS_CLOSED):
      {
        /* TCP connection already is about to be closed */
        /* #60 Close the connection while the TCP connection already is about to be closed. */
        /* do nothing, connection already is about to close */
        break;
      }
    default:
      {
        /* TCP connection is already closed ->  do nothing */
      }
    }

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #70 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_CLOSE, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  Tls_ProvideTxBuffer
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, TLS_CODE) Tls_ProvideTxBuffer(
    Tls_SocketIdType SocketId,
    P2VAR(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) *PbufPtr,
    uint32 Length )
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal  = BUFREQ_E_NOT_OK;
  uint8             errorId = TLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  else if (TLS_COND_INVALID_PTR(PbufPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    uint8 TConnId;

    TConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Check some error conditions before providing a buffer. */
    if(Length > Tls_TxPlainTextSizes[TConnId])
    {
      errorId = TLS_E_INV_PARAM;
      retVal = BUFREQ_E_OVFL;
    }
    /* check TLS connection state: */
    else if(TLS_STATE_TCP_ESTABL_TLS_ESTABL != Tls_ConnState[TConnId])
    {
      retVal = BUFREQ_E_NOT_OK;
    }
    /* check if buffer is already in use: */
    else if(0 != Tls_TxBufRequested[TConnId])
    {
      retVal = BUFREQ_E_BUSY;
    }
    /* #30 Provide the requested buffer. */
    else
    {
      Tls_TxBufRequested[TConnId]     = (uint16)Length;
      Tls_TxPbufList[TConnId].payload = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);
      Tls_TxPbufList[TConnId].len     = (uint16)Length;
      Tls_TxPbufList[TConnId].totLen  = Length;
      *PbufPtr = &Tls_TxPbufList[TConnId];

      retVal = BUFREQ_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_PROV_TX_BUFFER, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_TransmitTo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_TransmitTo(
    Tls_SocketIdType SocketId,
    P2CONST(Tls_PbufType, AUTOMATIC, TLS_CONST) PbufPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TLS_E_NO_ERROR;
  uint8          tConnId;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  else if (TLS_COND_INVALID_PTR(PbufPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    tConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Check if transmit request size is smaller or equals size as previously requested buffer. */
    if(PbufPtr->totLen > Tls_TxBufRequested[tConnId])
    {
      errorId = TLS_E_INV_PARAM;
    }
    else
    {
      /* Transmit size is smaller of equal to the previously requested buffer size. It doesn't matter if the size is
         smaller than previously requested as the stored (requested) length is only used for the length check and for
         the 'buffer is busy' check. */

      /* #30 Send data if the length is not 0. */
      if(0 != PbufPtr->totLen)
      {
        /* forward msg to TLS record layer (L1). */
        if(E_OK == Tls_VRl_Transmit(tConnId, Tls_TxPbufList[tConnId].payload, (uint16)Tls_TxPbufList[tConnId].totLen,
          TLS_REC_CONTENT_AD))
        {
          TLS_LOG_HS_DIV("Tls_TransmitTo: application data was sent");
          Tls_TxBufRequested[tConnId] = 0; /* free the TLS TxBuffer */

          /* add length to pending TxConfirmations */
          Tls_TxConfRemaining[tConnId] += (uint16)PbufPtr->totLen;
        }
        else
        {
          /* new attempt to send the data shall be made in the MainFunction */
        }
      }
      /* #40 Free the buffer if the length is 0. */
      else
      {
        /* nothing to be sent, DataLen is '0' */
        /* cancel / free the provided tx buffer */
        Tls_TxBufRequested[tConnId] = 0;
      }
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_TRANSMIT_TO, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_Received
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_Received(
    Tls_SocketIdType SocketId,
    uint32 Length )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TLS_E_NO_ERROR;
  uint8          tConnId;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    tConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    TLS_LOG_HS_DIV_2("Tls_Received: called with len %d while Tls_AppDataLenWaitingForRec is %d", Length, Tls_AppDataLenWaitingForRec[tConnId]);

    /* #20 Reduce the amount of stored data by the given length. */
    if(Tls_AppDataLenWaitingForRec[tConnId] >= Length)
    {
      Tls_AppDataLenWaitingForRec[tConnId] -= (uint16)Length;
    }
    else
    {
      /* error, erroneous length used in the call of this function */

      /* robust behavior: free the whole buffer */
      Tls_AppDataLenWaitingForRec[tConnId] = 0;
    }
    /* #30 Reset amount of used plain text buffer if all decrypted data is received by the upper layer. */
    if(0 == Tls_AppDataLenWaitingForRec[tConnId])
    {
      Tls_RxPlainTextFillLevel[tConnId] = 0;
    }
    retVal = E_OK;

  }
  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_RECEIVED, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  callback functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Tls_RxIndication
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TLS_CODE) Tls_RxIndication(
  Tls_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
  uint16 DataLen )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8  tConnId;
  uint8  errorId  = TLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  /* Check parameter 'DataPtr' */
  else if (TLS_COND_INVALID_PTR(DataPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    TLS_LOG_RX_IND_1("Tls_RxIndication: ----- new RxIndication received, len is %d", DataLen);

    tConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Append received data to Tls_RxStorage. */
    TLS_VENTER_CRITICAL_SECTION();  /* lock interrupts */  /* PRQA S 3109 */ /* MD_MSR_14.3 */
    (void)Tls_VWriteBufSegment(tConnId, DataPtr, DataLen, &Tls_RxBufSegMgt[tConnId]);
    TLS_VLEAVE_CRITICAL_SECTION();  /* unlock interrupts */  /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* Now the Tls_RxBufSegMgt contains some data, and this data can be detected and handled in the MainFunction.      */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_RX_INDICATION, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_TcpConnected
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TLS_CODE) Tls_TcpConnected(
    Tls_SocketIdType SocketId )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8  tConnId;
  uint8  errorId  = TLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    tConnId                = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Set connection state and trigger sending of client_hello. */
    Tls_ConnState[tConnId] = TLS_STATE_TCP_ESTABL_TLS_OPENING;

    TLS_LOG_HS_DIV_1("Tls_TcpConnect: Connected on socket %u", SocketId);
    TLS_LOG_HS_DIV_1("Tls_TcpConnect: connection state changed to %d", Tls_ConnState[tConnId]);

    /* now start TLS handshake */
    Tls_VHs_SendClientHello(tConnId);

  }
  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_TCP_CONNECTED, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_TcpIpEvent
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, TLS_CODE) Tls_TcpIpEvent(
    Tls_SocketIdType SocketId,
    IpBase_TcpIpEventType Event )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8  tConnId;
  uint8  errorId  = TLS_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  /* Check parameter 'DataPtr' */
  else if (Event > IPBASE_TCP_EVENT_FIN_RECEIVED)
  {
    errorId = TLS_E_INV_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    tConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Check if a connection is active on this socket. */
    if(TLS_COND_VALID_CONNIDX(tConnId) == TRUE)
    {
      /* #30 Start closing the TLS connection if a FIN was received on TCP. */
      if(IPBASE_TCP_EVENT_FIN_RECEIVED == Event)
      {
        /* indicate close request from peer to application */
        TLS_LOG_HS_DIV("Tls_TcpIpEvent: Event: initial FIN received from peer");

        /* indicate event, depending on current state */
        if(TLS_STATE_TCP_ESTABL_TLS_ESTABL == Tls_ConnState[tConnId])
        {
          /* unexpected close request of TCP connection */
          /* close TCP connection */
          Tls_ConnState[tConnId] = TLS_STATE_TCP_ESTABL_TLS_CLOSING;
        }

        /* close connection -> the TLS user has to / will close the connection */
      }
      /* #40 Reset the TLS connection if the TCP connection is fully closed. */
      else if(IPBASE_TCP_EVENT_CLOSED == Event)
      {
        /* indicate fully closed connection to application */
        TLS_LOG_HS_DIV("Tls_TcpIpEvent: Event: connection is fully closed now");

        /* reset TLS connection (internal structure) */
        Tls_VResetSockConnMappings(SocketId);
        Tls_VInitConnVars(tConnId);
      }
      /* #50 Reset the TLS connection if the TCP connection was reset. */
      else if(IPBASE_TCP_EVENT_RESET == Event)
      {
        /* indicate fully closed / resetted connection to application */
        TLS_LOG_HS_DIV("Tls_TcpIpEvent: Event: connection was RESET");

        /* reset TLS connection (internal structure) */
        Tls_VResetSockConnMappings(SocketId);
        Tls_VInitConnVars(tConnId);
      }
      else
      {
        /* invalid event -> do nothing */
      }
    }
    else
    {
      /* no active connection on this socket -> nothing to do */
    }

  }
  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #60 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_TCP_IP_EVENT, errorId);
  }
#else
  TLS_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Tls_VParseAllStoredServCerts
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VParseAllStoredServCerts( void )
{
  /* parse all stored server certs, so that these information can be used for checks during connection setup */
  switch(Tls_ServCertsParseStatus)
  {
  case TLS_SERV_CERT_PARSE_NONE:
    /* issue reading the first stored server certificate */
    if(TLS_NVM_ACCESS_IDLE == Tls_NvmExchStatus)
    {
      /* go through all server cert descriptors and check if the certs already have been parsed */
      uint8 NextCertIdx;
      for(NextCertIdx = 0; NextCertIdx < Tls_VCfgGetNumServerCerts(); NextCertIdx++)
      {
        if(FALSE == Tls_ServerCertDescs[NextCertIdx].CertWasParsed)
        {
          /* found a cert that was not yet parsed */
          break;
        }
      }
      if(NextCertIdx < Tls_VCfgGetNumServerCerts())
      {
        uint8 NvMReadState;
        Tls_ServCertsIdxActive = NextCertIdx;
        Tls_MainNvmRequBlockId = Tls_VCfgGetServerCertBlockIds(Tls_ServCertsIdxActive);

        /* get the status of the NvM block */
        (void)NvM_GetErrorStatus(Tls_MainNvmRequBlockId, &NvMReadState);  /* return value is always OK if input parameters are valid */
        if ( NVM_REQ_PENDING != NvMReadState )
        {
          Std_ReturnType nvmResult;

          Tls_NvmRetryCnt        = (uint8)Tls_VCfgGetMaxNumNvmRetryCnt();

          /* read NVM block */
          TLS_LOG_DIV_1("Tls_VParseAllStoredServCerts: trigger NvmRead for block %d", Tls_MainNvmRequBlockId);
          nvmResult = NvM_ReadBlock(Tls_MainNvmRequBlockId, &Tls_NvmExchData[0]);
          if(E_OK == nvmResult)
          {
            Tls_NvmExchStatus        = TLS_NVM_ACCESS_IN_USE;
            Tls_MainNvmRequStatus    = TLS_NVM_REQU_PEND;
          }
          else
          {
            /* triggering read request failed, try again later */
            TLS_LOG_DIV("Tls_VParseAllStoredServCerts: triggering first read request for server certs failed");
            Tls_MainNvmRequStatus    = TLS_NVM_REQU_RETRY;
            Tls_NvmRetryCnt--;
          }
          Tls_ServCertsParseStatus = TLS_SERV_CERT_PARSE_ACTIVE;
        }
        else
        {
          /* try again in the next MainFunction */
        }
      }
      else
      {
        /* error, function was triggered although all certs are already parsed */
      }
    }
    break;
  case TLS_SERV_CERT_PARSE_ACTIVE:
    if(Tls_MainNvmRequStatus == TLS_NVM_REQU_RETRY)
    {
      /* retry, issue reading a stored server certificate */
      uint8 NvMReadState;
      /* get the status of the NvM block */
      (void)NvM_GetErrorStatus(Tls_MainNvmRequBlockId, &NvMReadState);  /* return value is always OK if input parameters are valid */
      if ( NVM_REQ_PENDING != NvMReadState )
      {
        Std_ReturnType nvmResult;

        /* read NVM block */
        nvmResult = NvM_ReadBlock(Tls_MainNvmRequBlockId, &Tls_NvmExchData[0]);
        if(E_OK == nvmResult)
        {
          Tls_NvmExchStatus        = TLS_NVM_ACCESS_IN_USE;
          Tls_MainNvmRequStatus    = TLS_NVM_REQU_PEND;
        }
        else
        {
          /* triggering read request failed, try again later */
          Tls_MainNvmRequStatus    = TLS_NVM_REQU_RETRY;
          Tls_NvmRetryCnt--;

          if(0 == Tls_NvmRetryCnt)
          {
            /* error, TLS will not be operable */
            Tls_NvmExchStatus        = TLS_NVM_ACCESS_IDLE;
            Tls_MainNvmRequStatus    = TLS_NVM_REQU_IDLE;
            Tls_ServCertsParseStatus = TLS_SERV_CERT_PARSE_FAILED;
            TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_PARSE_ALL_STORED_SERVER_CERTS, TLS_EVENT_READ_ROOT_CERT_FROM_NVM, TLS_EVENT_STATUS_FAILED)  /* report event */
            TLS_LOG_DIV("Tls_VParseAllStoredServCerts: triggering read request for a server certs finally failed -> disable TLS");
            return;
          }
        }
      }
      else
      {
        /* block is busy, try again later */
      }
    }
    /* ---------------------------------------------------------------------------------------- */
    else if(Tls_MainNvmRequStatus == TLS_NVM_REQU_PEND)
    {
      /* check NVM error status */

      uint8 NvmResult;
      TLS_LOG_DIV_1("Tls_VParseAllStoredServCerts: Nvm request pending for block %d", Tls_VCfgGetServerCertBlockIds(Tls_ServCertsIdxActive));
      (void)NvM_GetErrorStatus(Tls_VCfgGetServerCertBlockIds(Tls_ServCertsIdxActive), &NvmResult);  /* return value is always OK if input parameters are valid */
      switch(NvmResult)
      {
      case NVM_REQ_OK:
      case NVM_REQ_NV_INVALIDATED:
        {
        uint8 NextCertIdx;
        boolean FurtherServerCertsAvailable;
        /* go ahead, now parse the certificate */
        TLS_LOG_DIV_1("Tls_VParseAllStoredServCerts: Nvm request succeded for block %d", Tls_VCfgGetServerCertBlockIds(Tls_ServCertsIdxActive));
        Tls_NvmRetryCnt = (uint8)Tls_VCfgGetMaxNumNvmRetryCnt();

        if(E_OK == Tls_VParseServerCert(&Tls_ServerCertDescs[Tls_ServCertsIdxActive], TLS_HASH_INFO_SUBJECT, &Tls_NvmExchData[0], TLS_NVM_EXCH_DATA_LEN))
        {
          /* cert is ok and could be parsed */
          /* nothing to be done */

          /* No validity period check is implemented for the certificate parsing at system startup. The validity check
             is done when estabishing a connecetion.
             There are two reasons for this decision:
             - It might be a problem to have a valid system time available at this early point of time.
             - The stored root certificate might become valid later and just not be valid yet, so marking it as invalid
               would prevent its later usage.
          */
        }
        else
        {
          /* cert could not be parsed successfully */
          Tls_ServerCertDescs[Tls_ServCertsIdxActive].Subject.ContLen = 0;  /* invalid length indicates problem with the cert */
        }
        Tls_ServerCertDescs[Tls_ServCertsIdxActive].CertWasParsed = TRUE;

        /* request next cert */
        {
          /* go through all server cert descriptors and check if the certs already have been parsed */
          FurtherServerCertsAvailable = FALSE;
          for(NextCertIdx = 0; NextCertIdx < Tls_VCfgGetNumServerCerts(); NextCertIdx++)
          {
            if(FALSE == Tls_ServerCertDescs[NextCertIdx].CertWasParsed)
            {
              /* found a cert that was not yet parsed */
              FurtherServerCertsAvailable = TRUE;
              break;
            }
          }
        }
        if(TRUE == FurtherServerCertsAvailable)
        {
          Std_ReturnType Ret;
          uint8 NvMReadState;

          Tls_ServCertsIdxActive = NextCertIdx;
          Tls_NvmRetryCnt = (uint8)Tls_VCfgGetMaxNumNvmRetryCnt();

          /* get the status of the NvM block */
          (void)NvM_GetErrorStatus(Tls_VCfgGetServerCertBlockIds(Tls_ServCertsIdxActive), &NvMReadState);  /* return value is always OK if input parameters are valid */
          if ( NVM_REQ_PENDING != NvMReadState )
          {
            /* request NVM block */
            Ret = NvM_ReadBlock(Tls_VCfgGetServerCertBlockIds(Tls_ServCertsIdxActive), &Tls_NvmExchData[0]);
            if(E_OK == Ret)
            {
              Tls_NvmExchStatus        = TLS_NVM_ACCESS_IN_USE;
              Tls_MainNvmRequStatus    = TLS_NVM_REQU_PEND;
            }
            else
            {
              /* triggering read request failed, try again later */
              Tls_MainNvmRequStatus    = TLS_NVM_REQU_RETRY;
              Tls_NvmRetryCnt--;
            }
          }
          else
          {
            /* block is busy, try again later */
          }
        }
        else
        {
          /* parsing the server certs is completed */
          uint8 CertIdx;
          Tls_ServCertsParseStatus = TLS_SERV_CERT_PARSE_FAILED;
          for(CertIdx = 0; CertIdx < Tls_VCfgGetNumServerCerts(); CertIdx++)
          {
            if(0 != Tls_ServerCertDescs[CertIdx].Subject.ContLen)
            {
              /* at least one cert is valid / could be parsed successfully */

              Tls_ServCertsParseStatus = TLS_SERV_CERT_PARSE_COMPLETE;
              break;
            }
          }

          /* Set error event if no valid root certificates could be found in NVM */
          if(TLS_SERV_CERT_PARSE_FAILED == Tls_ServCertsParseStatus)
          {
            TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_PARSE_ALL_STORED_SERVER_CERTS, TLS_EVENT_READING_ANY_ROOT_CERT_FROM_NVM, TLS_EVENT_STATUS_FAILED)  /* report event */
          }

          Tls_NvmExchStatus        = TLS_NVM_ACCESS_IDLE;
          Tls_MainNvmRequBlockId   = TLS_NVM_INV_BLOCK_ID;
          Tls_MainNvmRequStatus    = TLS_NVM_REQU_IDLE;
        }
        break;
        }
      case NVM_REQ_NOT_OK:
        /* repeat request if limit is not exceeded */
        Tls_NvmRetryCnt--;
        if(Tls_NvmRetryCnt > 0)
        {
          uint8 NvMReadState;
          /* get the status of the NvM block */
          (void)NvM_GetErrorStatus(Tls_VCfgGetServerCertBlockIds(Tls_ServCertsIdxActive), &NvMReadState);  /* return value is always OK if input parameters are valid */
          if ( NVM_REQ_PENDING != NvMReadState )
          {
            if(E_OK == NvM_ReadBlock(Tls_VCfgGetServerCertBlockIds(Tls_ServCertsIdxActive), &Tls_NvmExchData[0]))
            {
              Tls_ServerCertDescs[Tls_ServCertsIdxActive].CertFoundElements = 0;
              Tls_NvmExchStatus        = TLS_NVM_ACCESS_IN_USE;
              Tls_MainNvmRequStatus    = TLS_NVM_REQU_PEND;
            }
          }
          else
          {
            /* block is busy, try again later */
            Tls_NvmRetryCnt++;  /* revert decreased counter in case the block was busy */
          }
        }
        else
        {
          /* retry limit exceeded */
          Tls_ServCertsParseStatus = TLS_SERV_CERT_PARSE_FAILED;
          Tls_NvmExchStatus        = TLS_NVM_ACCESS_IDLE;
        }
        break;
      case NVM_REQ_PENDING:
        /* NVM request is still pending, data is not yet available */
        /* do nothing but wait */
        break;
      default:
        break;
      }
    }
    else
    {
      /* Tls_MainNvmRequStatus other than TLS_NVM_REQU_PEND or TLS_NVM_REQU_RETRY */
      /* do nothing */
    }
    break;
  default:
    /* status COMPLETE or FAILED */
    break;
  } /* end of switch */
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (TLS_SUPPORT_CRL == STD_ON)
/***********************************************************************************************************************
 *  Tls_VParseAllStoredCrls
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VParseAllStoredCrls( void )
{
  /* parse all stored CRLs, so that these infomrmation can be used for checks during connection setup */
  switch(Tls_CrlParseStatus)
  {
  case TLS_CRL_PARSE_NONE:
    /* issue reading the first stored CRL */
    if(TLS_NVM_ACCESS_IDLE == Tls_CrlNvmExchStatus)
    {
      /* go through all CRL descriptors and check if the CRLs already have been parsed */
      uint8 NextCrlIdx;
      for(NextCrlIdx = 0; NextCrlIdx < TLS_CFG_NUM_CRLS; NextCrlIdx++)
      {
        if(FALSE == Tls_CrlDescs[NextCrlIdx].CrlWasParsed)
        {
          /* found a CRL that was not yet parsed */
          break;  /* stop the for-loop */
        }
      }
      if(NextCrlIdx != TLS_CFG_NUM_CRLS)
      {
        Tls_CrlIdxActive            = NextCrlIdx;
        Tls_CrlMainNvmRequBlockId   = Tls_CfgCrlBlockIds[Tls_CrlIdxActive];
        Tls_CrlParseStatus          = TLS_CRL_PARSE_ACTIVE;
        Tls_CrlMainNvmRequStatus    = TLS_NVM_REQU_RETRY;
        Tls_CrlNvmRetryCnt          = (uint8)Tls_VCfgGetMaxNumNvmRetryCnt();

      }
      else
      {
        /* error, function was triggered although all CRLs are already parsed */
        Tls_CrlParseStatus = TLS_CRL_PARSE_COMPLETE;
        break;  /* stop the handling of this case */
      }
    }
    else
    {
      /* function was triggered while other NVM activites are ongoing */
      break;  /* stop the handling of this case, try again later */
    }
    /* do not call 'break' but fall through to the next state */

  case TLS_CRL_PARSE_ACTIVE:
    if(Tls_CrlMainNvmRequStatus == TLS_NVM_REQU_RETRY)
    {
      /* retry, issue reading a stored CRL */
      Std_ReturnType Ret;
      uint8 NvMReadState;
      /* get the status of the NvM block */
      (void)NvM_GetErrorStatus(Tls_CrlMainNvmRequBlockId, &NvMReadState);  /* return value is always OK if input parameters are valid */
      if ( NVM_REQ_PENDING != NvMReadState )
      {
        /* read NVM block */
        Ret = NvM_ReadBlock(Tls_CrlMainNvmRequBlockId, &Tls_CrlNvmExchData[0]);
        if(E_OK == Ret)
        {
          Tls_CrlNvmExchStatus        = TLS_NVM_ACCESS_IN_USE;
          Tls_CrlMainNvmRequStatus    = TLS_NVM_REQU_PEND;
        }
        else
        {
          /* triggering read request failed, try again later */
          Tls_CrlMainNvmRequStatus    = TLS_NVM_REQU_RETRY;
          Tls_CrlNvmRetryCnt--;

          if(0 == Tls_CrlNvmRetryCnt)
          {
            /* error, TLS will not be operable */
            Tls_CrlNvmExchStatus        = TLS_NVM_ACCESS_IDLE;
            Tls_CrlMainNvmRequStatus    = TLS_NVM_REQU_IDLE;
            Tls_CrlParseStatus          = TLS_CRL_PARSE_FAILED;
            TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_PARSE_ALL_STORED_CRLS, TLS_EVENT_READ_CRL_FROM_NVM, TLS_EVENT_STATUS_FAILED)  /* report event */
            TLS_LOG_DIV("Tls_VParseAllStoredCrls: triggering read request for a CRL finally failed -> disable TLS");
            return;
          }
        }
      }
      else
      {
        /* block is busy, try again later */
      }
    }
    /* ---------------------------------------------------------------------------------------- */
    else if(Tls_CrlMainNvmRequStatus == TLS_NVM_REQU_PEND)
    {
      /* check NVM error status */

      uint8 NvmResult;
      TLS_LOG_DIV_1("Tls_VParseAllStoredCrls: Nvm request pending for block %d", Tls_CfgCrlBlockIds[Tls_CrlIdxActive]);
      (void)NvM_GetErrorStatus(Tls_CfgCrlBlockIds[Tls_CrlIdxActive], &NvmResult);  /* return value is always OK if input parameters are valid */
      switch(NvmResult)
      {
      case NVM_REQ_OK:
      case NVM_REQ_NV_INVALIDATED:
        {
        uint8 NextCrlIdx;
        boolean FurtherCrlsAvailable;
        /* go ahead, now parse the CRL */
        TLS_LOG_DIV_1("Tls_VParseAllStoredCrls: Nvm request succeded for block %d", Tls_CfgCrlBlockIds[Tls_CrlIdxActive]);
        Tls_CrlNvmRetryCnt = (uint8)Tls_VCfgGetMaxNumNvmRetryCnt();

        if(E_OK == Tls_VParseCrl(Tls_CrlIdxActive, &Tls_CrlNvmExchData[0], TLS_CRL_NVM_EXCH_DATA_LEN))
        {
          /* CRL is ok and could be parsed */
          /* nothing to be done */
        }
        else
        {
          /* CRL could not be parsed successfully */
          Tls_CfgCrlElePosArrays[Tls_CrlIdxActive][TLS_CRL_ELE_ISSUER].ContLen = 0;  /* invalid length indicates problem with the CRL */
        }
        Tls_CrlDescs[Tls_CrlIdxActive].CrlWasParsed = TRUE;

        /* request next CRL */
        {
          /* go through all CRL descriptors and check if the CRLs already have been parsed */
          FurtherCrlsAvailable = FALSE;
          for(NextCrlIdx = 0; NextCrlIdx < TLS_CFG_NUM_CRLS; NextCrlIdx++)
          {
            if(FALSE == Tls_CrlDescs[NextCrlIdx].CrlWasParsed)
            {
              /* found a CRL that was not yet parsed */
              FurtherCrlsAvailable = TRUE;
              break;
            }
          }
        }
        if(TRUE == FurtherCrlsAvailable)
        {
          Std_ReturnType Ret;
          uint8 NvMReadState;

          Tls_CrlIdxActive = NextCrlIdx;
          Tls_CrlNvmRetryCnt = (uint8)Tls_VCfgGetMaxNumNvmRetryCnt();

          /* get the status of the NvM block */
          (void)NvM_GetErrorStatus(Tls_CfgCrlBlockIds[Tls_CrlIdxActive], &NvMReadState);  /* return value is always OK if input parameters are valid */
          if ( NVM_REQ_PENDING != NvMReadState )
          {
            /* request NVM block */
            Ret = NvM_ReadBlock(Tls_CfgCrlBlockIds[Tls_CrlIdxActive], &Tls_CrlNvmExchData[0]);
            if(E_OK == Ret)
            {
              Tls_CrlNvmExchStatus        = TLS_NVM_ACCESS_IN_USE;
              Tls_CrlMainNvmRequStatus    = TLS_NVM_REQU_PEND;
            }
            else
            {
              /* triggering read request failed, try again later */
              Tls_CrlMainNvmRequStatus    = TLS_NVM_REQU_RETRY;
              Tls_CrlNvmRetryCnt--;
            }
          }
          else
          {
            /* block is busy, try again later */
          }
        }
        else
        {
          /* parsing the CRLs is completed */
          uint8 CrlIdx;
          Tls_CrlParseStatus = TLS_CRL_PARSE_FAILED;
          for(CrlIdx = 0; CrlIdx < TLS_CFG_NUM_CRLS; CrlIdx++)
          {
            if(0 != Tls_CfgCrlElePosArrays[Tls_CrlIdxActive][TLS_CRL_ELE_ISSUER].ContLen)
            {
              /* at least one CRL is valid / could be parsed successfully */

              Tls_CrlParseStatus = TLS_CRL_PARSE_COMPLETE;
              break;
            }
          }

          /* set DET error if no valid CRLs could be found in NVM */
          if(TLS_CRL_PARSE_FAILED == Tls_CrlParseStatus)
          {
            TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_PARSE_ALL_STORED_CRLS, TLS_EVENT_READING_ANY_CRL_FROM_NVM, TLS_EVENT_STATUS_FAILED)  /* report event */
          }

          Tls_CrlNvmExchStatus        = TLS_NVM_ACCESS_IDLE;
          Tls_CrlMainNvmRequBlockId   = TLS_NVM_INV_BLOCK_ID;
          Tls_CrlMainNvmRequStatus    = TLS_NVM_REQU_IDLE;
        }
        break;
        }
      case NVM_REQ_NOT_OK:
        /* repeat request if limit is not exceeded */
        Tls_CrlNvmRetryCnt--;
        if(0 != Tls_CrlNvmRetryCnt)
        {
          uint8 NvMReadState;
          /* get the status of the NvM block */
          (void)NvM_GetErrorStatus(Tls_CfgCrlBlockIds[Tls_CrlIdxActive], &NvMReadState);  /* return value is always OK if input parameters are valid */
          if ( NVM_REQ_PENDING != NvMReadState )
          {
            if(E_OK == NvM_ReadBlock(Tls_CfgCrlBlockIds[Tls_CrlIdxActive], &Tls_CrlNvmExchData[0]))
            {
              Tls_CrlDescs[Tls_CrlIdxActive].CrlFoundElements = 0;
              Tls_CrlNvmExchStatus        = TLS_NVM_ACCESS_IN_USE;
              Tls_CrlMainNvmRequStatus    = TLS_NVM_REQU_PEND;
            }
          }
          else
          {
            /* block is busy, try again later */
            Tls_CrlNvmRetryCnt++;  /* revert decreased counter in case the block was busy */
          }
        }
        else
        {
          /* retry limit exceeded */
          Tls_CrlParseStatus       = TLS_CRL_PARSE_FAILED;
          Tls_CrlNvmExchStatus     = TLS_NVM_ACCESS_IDLE;
        }
        break;
      case NVM_REQ_PENDING:
        /* NVM request is still pending, data is not yet available */
        /* do nothing but wait */
        break;
      default:
        break;
      }
    }
    else
    {
      /* Tls_CrlMainNvmRequStatus other than TLS_NVM_REQU_PEND or TLS_NVM_REQU_RETRY */
      /* do nothing */
    }
    break;
  default:
    /* status COMPLETE or FAILED */
    break;
  } /* end of switch */
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif  /* (TLS_SUPPORT_CRL == STD_ON) */

/***********************************************************************************************************************
 *  Tls_VHs_SendClientHello
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientHello(
    uint8 TConnId )
{
  uint8 *TlsL2Ptr;
  uint16 Tls_L2Len;
  uint16 CurrentPos;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnVoid((TLS_COND_INIT), TLS_API_ID_V_HS_SEND_CLIENT_HELLO, TLS_E_NOT_INITIALIZED)
  TLS_CheckDetErrorReturnVoid((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_HS_SEND_CLIENT_HELLO, TLS_E_INV_PARAM)
#endif

  /* request internal buffer, fill with handshake message, then forward data to RECORD_LAYER */

  /* CLIENT HELLO -------------------------------------------------------------- */
  /* RFC 2246, section 7.4.1.2 */

  /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  /* handshake type: (1 byte) */
  TlsL2Ptr[TLS_HS_OFFSET_HS_TYPE]           = TLS_HS_TYPE_CLIENT_HELLO;

  /* length field (bytes 1..3) will be set later */

  /* protocol version: (2 byte) */
  TlsL2Ptr[TLS_HS_C_HELLO_OFFSET_MAJOR_VER] = TLS_MAJOR_VER;
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  TlsL2Ptr[TLS_HS_C_HELLO_OFFSET_MINOR_VER] = TLS_MINOR_VER_TLS12;
#else
  TlsL2Ptr[TLS_HS_C_HELLO_OFFSET_MINOR_VER] = TLS_MINOR_VER_TLS10;
#endif

  {
    /* fill RANDOM, this field has 32 byte ------------------------ */
    uint32 time32;

    Tls_GetTime32Fct(&time32);
    IPBASE_PUT_UINT32(TlsL2Ptr, TLS_HS_C_HELLO_OFFSET_RAND, time32);  /* 4 bytes of the RANDOM are the time code */

#if (TLS_ENABLE_STIR_RNG == STD_ON)
    /* stir RNG */
    IPBASE_PUT_UINT32(TlsL2Ptr, 10, Tls_MainFctCounter);  /* temporarily fill in the current MainFctCounter */
    (void)Crypto_StirRNG(20, &TlsL2Ptr[0]);  /* 4 byte time, 4 byte counter, 12 byte something else */  /* Use at least 20 bytes as seed for stirring the random genearator. So do not only use the time and the MainFctCounter. */
#endif  /* (TLS_ENABLE_STIR_RNG == STD_ON) */

    /* get data from RNG */
    (void)esl_getBytesRNG(28, &TlsL2Ptr[10]);  /* 28 bytes of the RANDOM are randomly generated */

    /* copy client random to internal struct */
    IpBase_Copy((IpBase_CopyDataType *)(&(Tls_SecParamTxPassivePtr[TConnId]->client_random)), (const IpBase_CopyDataType *)(&TlsL2Ptr[6]), TLS_HS_S_HELLO_RANDOM_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_struct */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
  }

  /* Using a session id is not supported by this implementation, so the length of this field is 0 and the ID does not exist */
  /* session ID len: */
  TlsL2Ptr[TLS_HS_C_HELLO_OFFSET_SESSION_ID_LEN] = 0;

  /* # Insert list of supported cipher suites. */
  /* cipher suites len: */
  IPBASE_PUT_UINT16(TlsL2Ptr, TLS_HS_C_HELLO_OFFSET_CIPHER_SUITE_LIST, (Tls_VCfgGetNumSuppCipherSuites() * 2));  /* Each cipher suite id has 2 bytes */  /*lint !e572 */
  /* cipher suite(s): */
  {
    uint8 SuiteIndex;
    CurrentPos = TLS_HS_C_HELLO_OFFSET_CIPHER_SUITE_LIST + 2;  /* start offset + 2 byte length field */
    for(SuiteIndex=0; SuiteIndex < Tls_VCfgGetNumSuppCipherSuites(); SuiteIndex++)
    {
      IPBASE_PUT_UINT16(TlsL2Ptr, CurrentPos, Tls_VCfgGetSuppCipherSuites(SuiteIndex));  /*lint !e572 */
      CurrentPos += 2;
    }
  }

  /* # Insert list of supported compression methods. */
  /* compression meth.s len */
  TlsL2Ptr[CurrentPos] = 1;
  /* compression meth. */
  TlsL2Ptr[CurrentPos+1] = TLS_COMPR_ALGORITHM_NULL;  /* none */
  CurrentPos += 2;

  /* # Insert client hello extensions. */
  {
    uint16 extStartPos = CurrentPos;

    /* write length later, so skip length field here */
    CurrentPos += 2;

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    /* # Insert supported signature algorithms extension. */
    {
      uint8  numSignAlgs      = 0;
      uint16 signAlgsStartPos = CurrentPos;

      TlsL2Ptr[CurrentPos + 0] = 0x00;  /* sign alg ext */
      TlsL2Ptr[CurrentPos + 1] = TLS_HS_HELLO_EXT_SIGN_ALGORITHMS;
      /* The size fields of this extension are filled in later, so for now skip these 4 bytes */
      CurrentPos += (2 + 4);
      /* Add all supported signature and hash algorithm combinations: */
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
      TlsL2Ptr[CurrentPos + 0] = TLS_HASH_ALGORITHM_SHA256;
      TlsL2Ptr[CurrentPos + 1] = TLS_SIGN_ALGORITHM_RSA;
      CurrentPos  += 2;
      numSignAlgs += 1;
      TlsL2Ptr[CurrentPos + 0] = TLS_HASH_ALGORITHM_SHA1;
      TlsL2Ptr[CurrentPos + 1] = TLS_SIGN_ALGORITHM_RSA;
      CurrentPos  += 2;
      numSignAlgs += 1;
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
      TlsL2Ptr[CurrentPos + 0] = TLS_HASH_ALGORITHM_SHA256;
      TlsL2Ptr[CurrentPos + 1] = TLS_SIGN_ALGORITHM_ECDSA;
      CurrentPos  += 2;
      numSignAlgs += 1;
      TlsL2Ptr[CurrentPos + 0] = TLS_HASH_ALGORITHM_SHA1;
      TlsL2Ptr[CurrentPos + 1] = TLS_SIGN_ALGORITHM_ECDSA;
      CurrentPos  += 2;
      numSignAlgs += 1;
#endif

      /* fill in extension size */
      TlsL2Ptr[signAlgsStartPos + 2] = 0x00;  /* extension length (2 byte)*/
      TlsL2Ptr[signAlgsStartPos + 3] = 2 + (numSignAlgs *2);
      TlsL2Ptr[signAlgsStartPos + 4] = 0x00;  /* list length (2 byte) */
      TlsL2Ptr[signAlgsStartPos + 5] = (numSignAlgs *2);
    }
#endif

#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
    /* # Insert Supported Elliptic Curves Extension */
    /* curve:  secp256r1 = prime256v1 = NIST P-256 = (0x0017)
       extension: 00 0A 00 04 00 02 00 17 */
    {
      CONST(uint8, TLS_CONST) NumEcCurves = 1;

      TlsL2Ptr[CurrentPos + 0] = 0x00;  /* elliptic curves ext */
      TlsL2Ptr[CurrentPos + 1] = TLS_HS_HELLO_EXT_ELLIPTIC_CURVES;
      TlsL2Ptr[CurrentPos + 2] = 0x00;  /* ext len (2 byte)*/
      TlsL2Ptr[CurrentPos + 3] = 2 + (NumEcCurves *2);
      TlsL2Ptr[CurrentPos + 4] = 0x00;  /* list len (2 byte) */
      TlsL2Ptr[CurrentPos + 5] = (NumEcCurves *2);
      /* curves: */
      TlsL2Ptr[CurrentPos + 6] = 0x00;  /* prime256v1 */ /* this is the only curve that is supported by this implementation */
      TlsL2Ptr[CurrentPos + 7] = 0x17;
      CurrentPos += (6 + (NumEcCurves *2));
    }

    /* # Insert Supported Point Formats Extension */
    /* format: uncompressed (0)
       extension: 00 0B 00 02 01 00 */
    {
      TlsL2Ptr[CurrentPos + 0] = 0x00;  /* point format ext */
      TlsL2Ptr[CurrentPos + 1] = TLS_HS_HELLO_EXT_EC_POINT_FORMATS;
      TlsL2Ptr[CurrentPos + 2] = 0x00;  /* ext len */
      TlsL2Ptr[CurrentPos + 3] = 0x02;
      TlsL2Ptr[CurrentPos + 4] = 0x01;  /* list len */
      TlsL2Ptr[CurrentPos + 5] = TLS_HELLO_EXT_ECC_POINT_FORMAT_UNCOMPRESSED;  /* list element: uncompressed */
      CurrentPos += 6;
    }
#endif

#if (TLS_SUPPORT_OCSP == STD_ON)
    /* # Insert OCSP status request extension. */
    /* support OCSP stapling */
    /* RFC 6066: TLS Extension Definitions */
    /* OCSP request */
    /* format: ocsp (1)
       extension: 00 05 00 05 01 00 00 00 00 */
    if((Tls_OcspWorkingMode[TConnId] == TLS_OCSP_MODE_SUPPORT_V1_V2) ||(Tls_OcspWorkingMode[TConnId] == TLS_OCSP_MODE_SUPPORT_V1))
    {
      TlsL2Ptr[CurrentPos + 0] = 0x00;  /* status_request ext */
      TlsL2Ptr[CurrentPos + 1] = TLS_HS_HELLO_EXT_OCSP_STATUS_REQUEST;
      TlsL2Ptr[CurrentPos + 2] = 0x00;  /* ext len */
      TlsL2Ptr[CurrentPos + 3] = 0x05;
      TlsL2Ptr[CurrentPos + 4] = 0x01;  /* status_type: ocsp */
      TlsL2Ptr[CurrentPos + 5] = 0x00;  /* responder list: empty */
      TlsL2Ptr[CurrentPos + 6] = 0x00;
      TlsL2Ptr[CurrentPos + 7] = 0x00;  /* extensions: empty */
      TlsL2Ptr[CurrentPos + 8] = 0x00;
      CurrentPos += 9;
    }

    /* # Insert OCSP status request V2 extension. */
    /* RFC 6961: Multiple Certificate Status Extension */
    /* OCSP request */
    /* format: ocsp (1), ocsp_multi(2)
       extension: 00 11 00 05 01 00 00 00 00 */
    if((Tls_OcspWorkingMode[TConnId] == TLS_OCSP_MODE_SUPPORT_V1_V2) ||(Tls_OcspWorkingMode[TConnId] == TLS_OCSP_MODE_SUPPORT_V2))
    {
      TlsL2Ptr[CurrentPos + 0] = 0x00;  /* status_request_v2 ext */
      TlsL2Ptr[CurrentPos + 1] = TLS_HS_HELLO_EXT_OCSP_STATUS_REQUEST_V2;
      TlsL2Ptr[CurrentPos + 2] = 0x00;  /* ext len */
      TlsL2Ptr[CurrentPos + 3] = 0x05;
      TlsL2Ptr[CurrentPos + 4] = 0x01;  /* status_type: ocsp(1), ocsp_multi(2) */
      TlsL2Ptr[CurrentPos + 5] = 0x00;  /* responder list: empty */
      TlsL2Ptr[CurrentPos + 6] = 0x00;
      TlsL2Ptr[CurrentPos + 7] = 0x00;  /* extensions: empty */
      TlsL2Ptr[CurrentPos + 8] = 0x00;
      CurrentPos += 9;
    }
#endif  /* TLS_SUPPORT_OCSP */

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
    /* # Insert TLS Heartbeat extension. */
    /* RFC 6520: TLS/DTLS Heartbeat Extension */
    /* format:
       - HeartbeatMode: peer_allowed_to_send(1), peer_not_allowed_to_send(2)
       - HeartbeatMessageType: heartbeat_request(1), heartbeat_response(2)
       - Hello Extension: heartbeat extension type (15)
       - Protocol: heartbeat content type (24)
       extension: 00 0F 00 01 02 */
    if(Tls_HeartbeatModeOwn[TConnId] != TLS_HB_MODE_NONE)
    {
      /* Inform the server that the client wants to send heartbeat messages */
      TlsL2Ptr[CurrentPos + 0] = 0x00;  /* heartbeat ext */
      TlsL2Ptr[CurrentPos + 1] = TLS_HS_HELLO_EXT_HEARTBEAT;
      TlsL2Ptr[CurrentPos + 2] = 0x00;  /* ext len (2 byte)*/
      TlsL2Ptr[CurrentPos + 3] = 1;
      TlsL2Ptr[CurrentPos + 4] = Tls_HeartbeatModeOwn[TConnId];

      CurrentPos += 5;
    }
#endif  /* TLS_SUPPORT_HEARTBEAT */

    /* # Insert 'Trusted CA' extension. */
    /* \trace CREQ-127025 */
    /* RFC 6066: TLS Extension Definitions
       format: pre_agreed(0), key_sha1_hash(1), x509_name(2), cert_sha1_hash(3) */
    {
      uint16       numberOfValidRootCerts = 0;  /* number of root cert elements that contain valid content */
      uint16       extensionLengthPos;          /* write position of the extension length field */
      uint8_least  rootCertIndex;               /* loop-index */

      /* Set the extension type (2 byte) */
      TlsL2Ptr[CurrentPos + 0] = 0x00;  /* trusted CA indication */
      TlsL2Ptr[CurrentPos + 1] = TLS_HS_HELLO_EXT_TRUSTED_CA_KEYS;
      /* The length of this extension and the length of the CA list will be set later */
      extensionLengthPos = CurrentPos + 2;
      CurrentPos += 6;

      /* Count all root certs that are successfully parsed and add their hash to the extension. */
      for(rootCertIndex = 0; rootCertIndex < TLS_CFG_NUM_SERVER_CERTS; rootCertIndex++)
      {
        /* Count only certs where some internal elements could be found. */
        if(Tls_ServerCertDescs[rootCertIndex].CertFoundElements != 0)
        {
          numberOfValidRootCerts++;
          TlsL2Ptr[CurrentPos] = TLS_EXT_TRUSTED_CA_IDENT_TYPE_CERT_SHA1_HASH;
          IpBase_Copy((IpBase_CopyDataType *)(&TlsL2Ptr[CurrentPos + 1]), (const IpBase_CopyDataType *)(&Tls_ServerCertDescs[rootCertIndex].HashSha1Cert[0]), TLS_HASH_SHA1_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_struct */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          CurrentPos += (TLS_HASH_SHA1_LEN + 1);
        }
      }

      /* Now set the length of this extension. */
      IPBASE_PUT_UINT16(TlsL2Ptr, extensionLengthPos,  (2 + (numberOfValidRootCerts * (TLS_HASH_SHA1_LEN + 1))));  /* ext len (2 byte)*/
      /* Now set the length of the CA list. */
      IPBASE_PUT_UINT16(TlsL2Ptr, (extensionLengthPos + 2), (numberOfValidRootCerts * (TLS_HASH_SHA1_LEN + 1)));   /* length of the list*/
   }

    /* Write overall extension length */
    IPBASE_PUT_UINT16(TlsL2Ptr, extStartPos, (CurrentPos - (extStartPos +2)));
  }  /* end of hello extensions */

  /* -> len = CurrentPos +5 -> used for buffer request */
  Tls_L2Len = CurrentPos;

  /* length: (3 byte) */ /* L2 length without msg type and this length field */
  TlsL2Ptr[1] = 0;  /* the message never is longer than 64k, so the first length byte is always '0' */
  IPBASE_PUT_UINT16(TlsL2Ptr, 2, (Tls_L2Len -4));

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, Tls_L2Len, TLS_REC_CONTENT_HS))  /* RL will prepend its header separately */
  {
    TLS_LOG_HS_DIV("Tls_VHs_SendClientHello: client hello was sent");
    Tls_HandShakeState[TConnId] = TLS_HS_STATE_HELLO_SENT;  /* state is only set to next value in case of success */

    /* Initializes hashes, and calculate hashes. */
#if ((TLS_SUPPORT_PROT_VER_TLS12 == STD_ON) && (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON))
    /* Both TLS1.0 and TLS1.2 are supported */
    {
      /* use ws1 for sha256 and ws2 for sha1+md5 */
      uint16 dataLen = Tls_L2Len;
      uint8 *dataPtr = TlsL2Ptr;

      /* init hashes */
      (void)esl_initSHA256( (eslt_WorkSpaceSHA256*)&Tls_WorkSpaceShaX_Hs1[TConnId] );  /* SHA256 WS init */
      (void)esl_initSHA1( (eslt_WorkSpaceSHA1*)&Tls_WorkSpaceShaX_Hs2[TConnId] );  /* SHA1 WS init */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      (void)esl_initMD5( &Tls_WorkSpaceMd5_Hs2[TConnId] );  /* MD5 WS init */

      /* update hashes */
      (void)esl_updateSHA256( (eslt_WorkSpaceSHA256*)&Tls_WorkSpaceShaX_Hs1[TConnId], dataLen, dataPtr );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      (void)esl_updateSHA1( (eslt_WorkSpaceSHA1*)&Tls_WorkSpaceShaX_Hs2[TConnId], dataLen, dataPtr );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      (void)esl_updateMD5( &Tls_WorkSpaceMd5_Hs2[TConnId], dataLen, dataPtr );
    }
#else
# if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    /* Only TLS1.2 is supported */
    {
      /* use ws1 for sha256 */
      uint16 dataLen = Tls_L2Len;
      uint8 *dataPtr = TlsL2Ptr;

      /* init hash */
      (void)esl_initSHA256( (eslt_WorkSpaceSHA256*)&Tls_WorkSpaceShaX_Hs1[TConnId] );  /* SHA256 WS init */
      /* update hash */
      (void)esl_updateSHA256( (eslt_WorkSpaceSHA256*)&Tls_WorkSpaceShaX_Hs1[TConnId], dataLen, dataPtr );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
    }
# endif
# if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
    /* Only TLS1.0 is supported */
    {
      /* use ws1 for sha1+md5 */
      uint16 dataLen = Tls_L2Len;
      uint8 *dataPtr = TlsL2Ptr;

      /* init hashes */
      (void)esl_initSHA1( (eslt_WorkSpaceSHA1*)&Tls_WorkSpaceShaX_Hs1[TConnId] );  /* SHA1 WS init */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      (void)esl_initMD5( &Tls_WorkSpaceMd5_Hs1[TConnId] );  /* MD5 WS init */
      /* update hashes */
      (void)esl_updateSHA1( (eslt_WorkSpaceSHA1*)&Tls_WorkSpaceShaX_Hs1[TConnId], dataLen, dataPtr );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      (void)esl_updateMD5( &Tls_WorkSpaceMd5_Hs1[TConnId], dataLen, dataPtr );
    }
# endif
#endif
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  Tls_VHs_SendClientKeyExchange
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientKeyExchangeRsa(
    uint8 TConnId )
{
  uint8 *TlsL2Ptr;
  uint16 Tls_L2Len;
  uint8 *PmsPassivePtr;
  uint16 EncLen;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnVoid((TLS_COND_INIT), TLS_API_ID_V_HS_SEND_CLIENT_KEY_EXCH_RSA, TLS_E_NOT_INITIALIZED)
  TLS_CheckDetErrorReturnVoid((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_HS_SEND_CLIENT_KEY_EXCH_RSA, TLS_E_INV_PARAM)
#endif

  /* request internal buffer, fill with handshake message, then forward data to RECORD_LAYER */

  /* CLIENT KEY EXCHANGE -------------------------------------------------------------- */

  /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  /* RSA key exchange */

  /* generate PMS: */
  {
    PmsPassivePtr = Tls_SecParamTxPassivePtr[TConnId]->master_secret;  /* temporary fill PMS into storage of MS */

    PmsPassivePtr[0] = TLS_MAJOR_VER;
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    PmsPassivePtr[1] = TLS_MINOR_VER_TLS12;  /* the same version as it was offerd in client_hello. */
#else
    PmsPassivePtr[1] = TLS_MINOR_VER_TLS10;
#endif
    {
      uint8 RandArray[20];

      /* take some random data */
      Tls_GetTime32Fct((uint32*)(&RandArray[0]));  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_x */  /* PRQA S 3305 */ /* MD_TLS_3305_param */
      IPBASE_PUT_UINT32(&RandArray[0], 4, Tls_MainFctCounter);  /* temorary fill in the current MainFctCounter */
      /* stir RNG */
      (void)Crypto_StirRNG(20, &RandArray[0]);  /* 4 byte time, 4 byte counter, 12 byte something */
      (void)Crypto_StirRNG(TLS_C_HELLO_RAND_LEN, &(Tls_SecParamTxPassivePtr[TConnId]->server_random[0]));

      /* fill 46 byte with random data */
      /* get data from RNG */
      (void)esl_getBytesRNG(46, &PmsPassivePtr[2]);
    }
  }

  /* encrypt PMS with public key of the server */
  {
    eslt_WorkSpaceRSAenc  *WorkSpaceRsaEncPtr;
    uint8 *ModulusPtr;
    uint8 *ExponentPtr;
    eslt_ErrorCode ErrorCode;

    WorkSpaceRsaEncPtr = (eslt_WorkSpaceRSAenc *)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
    ModulusPtr         = (uint8 *)(&(Tls_ServEncKey[TConnId * Tls_VCfgGetMaxServerCertKeySizeByte()]));
    ExponentPtr        = &(Tls_RsaExp[TConnId][0]);

    /* init RSA workspace */
    ErrorCode = esl_initWorkSpaceHeader(&WorkSpaceRsaEncPtr->header, ESL_MAXSIZEOF_WS_RSA_ENC, NULL_PTR);  /* PRQA S 3393,3395 */ /* MD_TLS_12.1_3393_3395 */
    if (ESL_ERC_NO_ERROR != ErrorCode)
    {
      TLS_LOG_HS_DIV_1("Tls_VHs_SendClientKeyExchangeRsa: Init workspace header failed, error code is '0x%X'", ErrorCode);
      return;
    }

    ErrorCode = esl_initEncryptRSA_V15(WorkSpaceRsaEncPtr, Tls_RsaKeyLen[TConnId], ModulusPtr, Tls_RsaExpLen[TConnId], ExponentPtr);
    if( ESL_ERC_NO_ERROR != ErrorCode)
    {
      TLS_LOG_HS_DIV_1("Tls_VHs_SendClientKeyExchangeRsa: Init RSA failed, error code is '0x%X'", ErrorCode);
      return;
    }

    /* now encrypt the PMS using RSA, write it directly into the buffer */
    EncLen = TLS_MAX_SUPP_RSA_KEY_LEN_BYTE;
    ErrorCode = esl_encryptRSA_V15(WorkSpaceRsaEncPtr, TLS_MASTER_SECRET_LEN, &PmsPassivePtr[0], &EncLen, &TlsL2Ptr[4+2]);
    if( ESL_ERC_NO_ERROR != ErrorCode)
    {
      TLS_LOG_HS_DIV_2("Tls_VHs_SendClientKeyExchangeRsa: error encrypting PMS, error code is '0x%X', len is %d", ErrorCode, EncLen);
      return;
    }

  }

  /* write the handshake type: (1 byte) */
  TlsL2Ptr[0] = TLS_HS_TYPE_CLIENT_KEY_EXCH;
  /* write the length of this handshake message: (3 byte) */
  TlsL2Ptr[1] = 0;
  IPBASE_PUT_UINT16(TlsL2Ptr, 2, (EncLen +2));  /* L2 length without msg type and this length field. The length will never exceed 64k. */
  /* write size of the key */
  IPBASE_PUT_UINT16(TlsL2Ptr, 4, (EncLen));     /* byte size of the exchanged key (2 byte field), 0x0100 = 256(dec) -> 2048 bit key is used */
  Tls_L2Len = EncLen + 6;

  /* copy and update hash workspaces */
  Tls_VHs_HashesCopyAndUpdate(TConnId, Tls_L2Len, TlsL2Ptr);

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, Tls_L2Len, TLS_REC_CONTENT_HS))  /* RL will prepend its header separately) */
  {
    uint8 Seed[2*TLS_HS_RANDOM_LEN];
    uint8 SeedLen = 2*TLS_HS_RANDOM_LEN;

    TLS_LOG_HS_DIV("Tls_VHs_SendClientKeyExchangeRsa: client key exchange was sent");

    /* now calculate the master secret */
    {
      Std_ReturnType Ret;
      uint8 TempMasterSecret[TLS_MASTER_SECRET_LEN];
      uint8 *SecretPtr;
      SecretPtr = PmsPassivePtr;
      IpBase_Copy((IpBase_CopyDataType*)(&Seed[0]), (const IpBase_CopyDataType*)(&(Tls_SecParamTxPassivePtr[TConnId]->client_random[0])), TLS_HS_RANDOM_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
      IpBase_Copy((IpBase_CopyDataType*)(&Seed[TLS_HS_RANDOM_LEN]), (const IpBase_CopyDataType*)(&(Tls_SecParamTxPassivePtr[TConnId]->server_random[0])), TLS_HS_RANDOM_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
      Ret = Tls_VPrf( TConnId, &TempMasterSecret[0], SecretPtr, (const uint8 *)&Tls_Hs_MasterSecretLabel[0], &Seed[0],
                      TLS_MASTER_SECRET_LEN, TLS_MASTER_SECRET_LEN, TLS_LABEL_MASTER_SECRET_LEN, SeedLen );
      if(E_OK != Ret)
      {
        return;
      }
      /* copy from TempMasterSecret to MasterSecret */
      IpBase_Copy((IpBase_CopyDataType*)SecretPtr, (IpBase_CopyDataType*)(&TempMasterSecret[0]), TLS_MASTER_SECRET_LEN);

      /* copy master secret also to rx passive parameters */
      IpBase_Copy( (IpBase_CopyDataType*)(&(Tls_SecParamRxPassivePtr[TConnId]->master_secret[0])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
                   (const IpBase_CopyDataType*)(&(Tls_SecParamTxPassivePtr[TConnId]->master_secret[0])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
                   TLS_MASTER_SECRET_LEN );
    }

    Tls_HandShakeState[TConnId] = TLS_HS_STATE_CL_KEY_EXCH_SENT;
    return;
  }
  else
  {
    /* send failed, so revert to old hash ws (copy hash workspaces) and do not set HandshakeState to next value */
    Tls_VHs_HashesRevert(TConnId);
    return;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  Tls_VHs_SendClientKeyExchangeEcdh
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientKeyExchangeEcdh(
    uint8 TConnId )
{
  uint8  *TlsL2Ptr;
  uint16  TlsL2Len;
  uint16  KeyLen;
  /* sizes for max supported ECC curves */
  uint8  *PrivKeyPtr;  /* only x-component */
  eslt_ErrorCode RetCv;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnVoid((TLS_COND_INIT), TLS_API_ID_V_HS_SEND_CLIENT_KEY_EXCH_ECDH, TLS_E_NOT_INITIALIZED)
  TLS_CheckDetErrorReturnVoid((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_HS_SEND_CLIENT_KEY_EXCH_ECDH, TLS_E_INV_PARAM)
#endif

  /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  {
    eslt_WorkSpaceEcP        *EcWorkSpPtr     = (eslt_WorkSpaceEcP *)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
    const eslt_EccDomain     *EcDomainPtr     = &Crypto_EccCurveANSIp256r1Domain[0];
    const eslt_EccDomainExt  *EcDomainExtPtr  = &Crypto_EccCurveANSIp256r1DomainExt[0];
    const eslt_EccSpeedUpExt *EcSpeedUpExtPtr = &Crypto_EccCurveANSIp256r1SpeedUpExt[0];  /* 547 byte */
    uint8   PubKeyLen;
    uint8  *PubKeyXPtr;
    uint8  *PubKeyYPtr;
    uint8  *PubKeyPtr;

  /* request internal buffer, fill with handshake message, then forward data to RECORD_LAYER */

  /* CLIENT KEY EXCHANGE -------------------------------------------------------------- */

  {
    /* ECC key exchange */

    /* if no client cert was sent or cert does not contain all necessary information -> generate key and send info to server */

    /* Private keys are interpreted as an array of bytes with length according to
       esl_getLengthOfEcPprivateKey(), which is the number of bytes of the order of the base point.
       The esl_generateKeyEcP() function delivers an appropriate private key as output.
       The coordinates of public keys are stored separately. The coordinates are interpreted as arrays of
       bytes with length according to esl_getLengthOfEcPpublicKey_comp() which is the number of bytes
       of the prime.
       The function esl_generateKeyEcP() delivers an appropriate public key as output. */

    /*
     eslt_Length esl_getMaxLengthOfEcPmessage     ( eslt_EccDomain ROM *domain );
     -> the maximum possible byte length of the message,

     eslt_Length esl_getLengthOfEcPpublicKey_comp ( eslt_EccDomain ROM *domain );
     -> the byte length of the public key component,

     eslt_Length esl_getLengthOfEcPprivateKey     ( eslt_EccDomain ROM *domain );
     -> the byte length of the private key,

     eslt_Length esl_getLengthOfEcPsignature_comp ( eslt_EccDomain ROM *domain );
     -> the byte length of a signature component and

     eslt_Length esl_getLengthOfEcPsecret_comp    ( eslt_EccDomain ROM *domain );
     -> the byte length of a component of the common secret value
    */

    if( (ESL_ERC_NO_ERROR != esl_initWorkSpaceHeader(&EcWorkSpPtr->header, ESL_MAXSIZEOF_WS_ECP, NULL_PTR)) ||  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
        (ESL_ERC_NO_ERROR != esl_initGenerateKeyEcP_prim(EcWorkSpPtr, EcDomainPtr, EcDomainExtPtr, EcSpeedUpExtPtr)) )
    {
      return;
    }
    PubKeyLen  = (uint8)esl_getLengthOfEcPpublicKey_comp(EcDomainPtr);

    PrivKeyPtr = &Tls_PrivEccKeyArray[TConnId * TLS_MAX_SUPP_ECC_KEY_COMP_SIZE];
    PubKeyPtr  = &TlsL2Ptr[TLS_HS_PREFIX_LEN];
    PubKeyXPtr = &PubKeyPtr[2];
    PubKeyYPtr = &PubKeyPtr[2 + PubKeyLen];
    KeyLen = (2 * PubKeyLen) +2;
    RetCv = esl_generateKeyEcP_prim(EcWorkSpPtr, PrivKeyPtr, PubKeyXPtr, PubKeyYPtr);
    if(ESL_ERC_NO_ERROR != RetCv)
    {
      return;
    }

  }

  PubKeyPtr[0] = (uint8)(KeyLen -1);  /* length of point element */
  PubKeyPtr[1] = TLS_ECC_PUB_KEY_POINT_FORMAT_UNCOMPRESSED;  /* uncompressed point */

  }

  /* handshake type: (1 byte) */
  TlsL2Ptr[0] = TLS_HS_TYPE_CLIENT_KEY_EXCH;
  TlsL2Len    = KeyLen + TLS_HS_PREFIX_LEN;

  /* length: (3 byte) */
  TlsL2Ptr[1] = 0;
  IPBASE_PUT_UINT16(TlsL2Ptr, 2, KeyLen);  /* L2 length without msg type and this length field. The length will never exceed 64k. */

  /* copy and update hash workspaces */
  Tls_VHs_HashesCopyAndUpdate(TConnId, TlsL2Len, TlsL2Ptr);

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, TlsL2Len, TLS_REC_CONTENT_HS))  /* RL will prepend its header separatel) */
  {
    if(FALSE == Tls_ClientCertRequested[TConnId])
    {
      if(E_OK != Tls_VHs_CalcEcdhMasterSecret(TConnId, PrivKeyPtr))
      {
        /* under normal conditions this error should never happen, since only a secret generation and HMAC calcualtions
        are executed, without any random input or other possibly not available elements */
#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
        TLS_CallDetReportError(TLS_API_ID_V_HS_SEND_CLIENT_KEY_EXCH_ECDH, TLS_E_UNEXPECTED_CRYPTO_ERROR)
#endif
          /* do not switch to next handshake state */
        return;
      }
    }

    Tls_HandShakeState[TConnId] = TLS_HS_STATE_CL_KEY_EXCH_SENT;
    return;  /* sending was successful */
  }
  else
  {
    /* send failed, so revert to old hash ws (copy hash workspaces) and do not set HandshakeState to next value */
    Tls_VHs_HashesRevert(TConnId);
    return;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  Tls_VHs_CalcEcdhMasterSecret
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_CalcEcdhMasterSecret(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) PrivKeyPtr )
{
  eslt_ErrorCode RetCv;
  eslt_WorkSpaceEcP        *EcWorkSpPtr     = (eslt_WorkSpaceEcP *)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
  const eslt_EccDomain     *EcDomainPtr     = &Crypto_EccCurveANSIp256r1Domain[0];
  const eslt_EccDomainExt  *EcDomainExtPtr  = &Crypto_EccCurveANSIp256r1DomainExt[0];
  uint8 *ServPubKeyXPtr;
  uint8 *ServPubKeyYPtr;

  uint8 Seed[2*TLS_HS_RANDOM_LEN];
  uint8 SeedLen = 2*TLS_HS_RANDOM_LEN;

  uint8  SecretXandY[TLS_ECDH_SECRET_LEN_XY_256BIT];  /* This size is curve dependent! Currently only the 256bit curve ANSIp256r1 is supported. */
  Std_ReturnType Ret;
  uint8 *PmsPtr;
  uint8 *MsPtr;
  uint8 PmsLen;

  TLS_LOG_HS_DIV("Tls_VHs_SendClientKeyExchange: client key exchange was sent");

  /* now calculate the master secret ------------------------------------------------------------ */

  /* calc shared secret, and then derive master secret (in usual way) */

  if( (ESL_ERC_NO_ERROR != esl_initWorkSpaceHeader(&EcWorkSpPtr->header, ESL_MAXSIZEOF_WS_ECP, NULL_PTR)) ||  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
      (ESL_ERC_NO_ERROR != esl_initGenerateSharedSecretDHEcP_prim(EcWorkSpPtr, EcDomainPtr, EcDomainExtPtr)) )
  {
    return E_NOT_OK;
  }

  ServPubKeyXPtr = &Tls_ServEncKey[(TConnId * Tls_VCfgGetMaxServerCertKeySizeByte()) +1 +  0];
  ServPubKeyYPtr = &Tls_ServEncKey[(TConnId * Tls_VCfgGetMaxServerCertKeySizeByte()) +1 + TLS_ECC_P256R1_COORDINATE_SIZE];

  RetCv = esl_generateSharedSecretDHEcP_prim( EcWorkSpPtr, PrivKeyPtr, ServPubKeyXPtr, ServPubKeyYPtr, &SecretXandY[0], &SecretXandY[TLS_ECC_P256R1_COORDINATE_SIZE]);
  if(ESL_ERC_NO_ERROR != RetCv)
  {
    return E_NOT_OK;
  }

  PmsPtr = &SecretXandY[0];
  PmsLen = 32;  /* only the x-component */
  MsPtr = &(Tls_SecParamTxPassivePtr[TConnId]->master_secret[0]);

  IpBase_Copy((IpBase_CopyDataType*)(&Seed[0]), (const IpBase_CopyDataType*)(&(Tls_SecParamTxPassivePtr[TConnId]->client_random[0])), TLS_HS_RANDOM_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
  IpBase_Copy((IpBase_CopyDataType*)(&Seed[TLS_HS_RANDOM_LEN]), (const IpBase_CopyDataType*)(&(Tls_SecParamTxPassivePtr[TConnId]->server_random[0])), TLS_HS_RANDOM_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
  Ret = Tls_VPrf( TConnId,
    MsPtr, PmsPtr, &Tls_Hs_MasterSecretLabel[0], &Seed[0],
    TLS_MASTER_SECRET_LEN, PmsLen, TLS_LABEL_MASTER_SECRET_LEN, SeedLen );
  if(E_OK != Ret)
  {
    return E_NOT_OK;
  }

  /* copy master secret also to rx passive parameters */
  IpBase_Copy( (IpBase_CopyDataType*)(&(Tls_SecParamRxPassivePtr[TConnId]->master_secret[0])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
    (const IpBase_CopyDataType*)(&(Tls_SecParamTxPassivePtr[TConnId]->master_secret[0])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
    TLS_MASTER_SECRET_LEN );

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/***********************************************************************************************************************
 *  Tls_VHs_SendClientCert
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientCert(
    uint8 TConnId )
{
  uint8 *TlsL2Ptr;
  uint16 Tls_L2Len;
  uint16 CertLen;
  uint8 *CertPtr;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnVoid((TLS_COND_INIT), TLS_API_ID_V_HS_SEND_CLIENT_CERT, TLS_E_NOT_INITIALIZED)
  TLS_CheckDetErrorReturnVoid((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_HS_SEND_CLIENT_CERT, TLS_E_INV_PARAM)
#endif

  CertPtr = Tls_ClientCertPtr[TConnId];
  CertLen = Tls_ClientCertLen[TConnId];  /* parsed from cert */

  if(0 != CertLen)
  {
    /* get info from KeyMat */
    CONST(uint8, TLS_CONST)     EleNrCertRoot[1]    = { 1 };  /* chapter 1: root of the key material */
    CONST(uint8, TLS_CONST)     MaxEleDepth         = 1;
    IpBase_ReturnType           Res;
    IpBase_BerWorkspaceType     Workspace;
    IpBase_BerStackElementType  BerStack[1];  /* max depth of one element */
    IpBase_BerElementType       EleDesc;
    uint32                      DataLen = CertLen;  /* max */

    IpBase_BerInitWorkspace(&Workspace, &BerStack[0], MaxEleDepth); /* only needed once */

    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrCertRoot[0], 1, CertPtr, DataLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      CertLen = (uint16)(EleDesc.EndIdx - EleDesc.Idx);
      Tls_ClientCertLen[TConnId] = CertLen;
    }
  }

  /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  /* handshake type: (1 byte) */
  TlsL2Ptr[0] = TLS_HS_TYPE_CERTIFICATE;

  /* length of message depends on length of client certificate */
  if(0 != CertLen)
  {
    Tls_L2Len = 1 + 9 + CertLen;  /* only one single cert will be supported in this impl */

    /* length: (3 byte) */
    TlsL2Ptr[1] = 0;
    TlsL2Ptr[2] = (uint8)(((CertLen +6) >> 8) & 0xFF);  /* L2 length without msg type and this length field */
    TlsL2Ptr[3] = (uint8)(((CertLen +6) >> 0) & 0xFF);

    /* certificates lengths: (each 3 byte) */
    TlsL2Ptr[4] = 0;
    TlsL2Ptr[5] = (uint8)(((CertLen +3) >> 8) & 0xFF);  /* length of all certificates */
    TlsL2Ptr[6] = (uint8)(((CertLen +3) >> 0) & 0xFF);

    /* only one single cert supported, currently no cert chain is supported */
    TlsL2Ptr[7] = 0;
    TlsL2Ptr[8] = (uint8)(((CertLen) >> 8) & 0xFF);  /* length of the client certificate */
    TlsL2Ptr[9] = (uint8)(((CertLen) >> 0) & 0xFF);

    /* copy certificate to buffer */
    IpBase_Copy((IpBase_CopyDataType*)(&TlsL2Ptr[1+9]), (const IpBase_CopyDataType*)(CertPtr), CertLen);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
  }
  else
  {
    /* there is no client cert configured, so send an empty cert message */
    Tls_L2Len = 1 + 6 + 0;

    /* length: (3 byte) */
    TlsL2Ptr[1] = 0;  /* L2 length without msg type and this length field (=3) */
    TlsL2Ptr[2] = 0;
    TlsL2Ptr[3] = 3;

    /* certificates lengths: (each 3 byte) */
    TlsL2Ptr[4] = 0;  /* length of all certificates (=0) */
    TlsL2Ptr[5] = 0;
    TlsL2Ptr[6] = 0;
  }

  /* copy and update hash workspaces */
  Tls_VHs_HashesCopyAndUpdate(TConnId, Tls_L2Len, TlsL2Ptr);

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, Tls_L2Len, TLS_REC_CONTENT_HS))  /* RL will prepend its header separatel) */
  {
    TLS_LOG_HS_DIV("Tls_VHs_SendClientCert: client certificate was sent");

    Tls_HandShakeState[TConnId] = TLS_HS_STATE_CERT_SENT;
  }
  else
  {
    /* send failed, so revert to old hash ws (copy hash workspaces) and do not set HandshakeState to next value */
    Tls_VHs_HashesRevert(TConnId);
  }
  return;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (TLS_SUPPORT_CLIENT_CERT == STD_ON)
/***********************************************************************************************************************
 *  Tls_VHs_SendClientCertVeri
 **********************************************************************************************************************/
/*! \brief      Send a client_certificate_verify
 *  \param[in]  TConnId        TLS connection id
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientCertVeri(
    uint8 TConnId )
{
  uint8 *TlsL2Ptr;
  uint16 Tls_L2Len;
  /* SignInput[] has a size that can hold the value to be signed for several usecases: TLS1.0 (md5+sha1 =36byte),
     TLS1.2 with ECDH (sha256+2byte =34byte), TLS1.2 with RSA (BER-coded hash alg + hash value =51byte) */
  uint8  SignInput[TLS_CERT_VERI_RSA_SHA256_SIGN_DESCR_LEN + TLS_HASH_SHA256_LEN];
  uint16 SignLen;
  uint8  ExtraLenHashAndSign;
  uint8  HashOffset;
  uint8  HashSize = 0;  /* Variable is initialized to avoid compiler warnings. The variable will now always be overwritten before it is read. */

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnVoid((TLS_COND_INIT), TLS_API_ID_V_HS_SEND_CLIENT_CERT_VERI, TLS_E_NOT_INITIALIZED)
  TLS_CheckDetErrorReturnVoid((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_HS_SEND_CLIENT_CERT_VERI, TLS_E_INV_PARAM)
#endif

  TLS_LOG_HS_DIV("Tls_VHs_SendClientCertVeri: function was called");

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
#  if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  if(Tls_ProtVer[TConnId] == TLS_PROT_VER_TLS12)
# endif
  {
    /* TLS 1.2 */
    ExtraLenHashAndSign = TLS_HS_C_CERT_VERI_TLS12_EXTRA_OFFSET;

#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
# if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
    if(Tls_Hs_KeyExchType[TConnId] == TLS_KEY_EXCH_TYPE_RSA)
# endif
    {
      /* key exchange type is RSA */
      HashOffset = TLS_CERT_VERI_RSA_SHA256_SIGN_DESCR_LEN;
      HashSize   = TLS_HASH_SHA256_LEN;
    }
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
# if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
    else
# endif
    {
      /* key exchange type is ECDH */
      HashOffset = TLS_HS_C_CERT_VERI_TLS12_EXTRA_OFFSET;
      HashSize   = TLS_HASH_SHA256_LEN;
    }
#endif
  }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
#  if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  else
# endif
  {
    /* TLS 1.0 */
    ExtraLenHashAndSign = 0;
    HashOffset          = 0;
  }
#endif

  /* finalize hash of HS messages */
  /* hash2 is a copy of hash1. hash2 will be closed here, hash1 will be contiued. */
  switch(Tls_HsHashAlg[TConnId])
  {
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  case TLS_HS_HASH_ALG_MD5SHA1:
    {
      /* only for TLS 1.0 */
      /* calculate hash with MD5 and SHA1, and put both into one array */
      eslt_ErrorCode ErrorCode;

      /* MD5 finalization */
      IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        (const IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        sizeof(Tls_WorkSpaceMd5_Hs1[TConnId]) );
      ErrorCode = esl_finalizeMD5( &Tls_WorkSpaceMd5_Hs2[TConnId], &SignInput[0] );
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return;
      }

      /* SHA1 finalization */
      IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        sizeof(eslt_WorkSpaceSHA1) );
      ErrorCode = esl_finalizeSHA1( (eslt_WorkSpaceSHA1*)&Tls_WorkSpaceShaX_Hs2[TConnId], &SignInput[TLS_HASH_MD5_LEN] );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return;
      }
      break;
    }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  case TLS_HS_HASH_ALG_SHA256:
    {
      /* calculate hash with SHA256 */
      eslt_ErrorCode ErrorCode;
      /* SHA256 finalization */
      IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        sizeof(eslt_WorkSpaceSHA256) );
      ErrorCode = esl_finalizeSHA256( (eslt_WorkSpaceSHA256*)&Tls_WorkSpaceShaX_Hs2[TConnId], &SignInput[0 + HashOffset] );
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return;
      }
      break;
    }
#endif
  default:
    {
      /* error, invalid hash alg */
      return;
    }
  }

  /* --------------------------------------------------------------------------------- */
  /* sign data using the clients private key */

  /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  /* insert the handshake type: (1 byte) */
  TlsL2Ptr[TLS_HS_OFFSET_HS_TYPE] = TLS_HS_TYPE_CERT_VERIFY;
  /* the length field (byte 1..3) will be set later */

  /* insert the signature depending on the signature type */
  switch(Tls_Hs_KeyExchType[TConnId])
  {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
  case(TLS_KEY_EXCH_TYPE_RSA):
    {
      Tls_VHs_AddRsaSignToClientCertVeriMsg( TConnId, TlsL2Ptr, HashOffset, HashSize, ExtraLenHashAndSign, &SignInput[0], &SignLen );
      break;
    }
#endif
    /* TLS_KEY_EXCH_RSA_SUPPORT */

#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
  case(TLS_KEY_EXCH_TYPE_ECDH):
    {
      Tls_VHs_AddEcdsaSignToClientCertVeriMsg( TConnId, TlsL2Ptr, /* HashOffset, HashSize,*/ ExtraLenHashAndSign, &SignInput[0], &SignLen );
      break;
    }
#endif
      /* TLS_KEY_EXCH_ECDH_SUPPORT */
  default:
    {
      /* invalid key exchange type */
      return;  /* error */
    }
  }

  /* length of message depends on length of client key and cipher suite
     (e.g. 1024bit RSA key -> 128byte signature length) */

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  Tls_L2Len = 6 + ExtraLenHashAndSign + SignLen;
  /* length: (3 byte) */
  TlsL2Ptr[1] = 0;
  TlsL2Ptr[2] = (uint8)(((SignLen + ExtraLenHashAndSign +2) >> 8) & 0xFF);  /* L2 length without msg type and this length field */
  TlsL2Ptr[3] = (uint8)(((SignLen + ExtraLenHashAndSign +2) >> 0) & 0xFF);
#else
  Tls_L2Len = 6 + SignLen;
  /* length: (3 byte) */
  TlsL2Ptr[1] = 0;
  TlsL2Ptr[2] = (uint8)(((SignLen + 2) >> 8) & 0xFF);  /* L2 length without msg type and this length field */
  TlsL2Ptr[3] = (uint8)(((SignLen + 2) >> 0) & 0xFF);
#endif

  /* copy and update hash workspaces */
  Tls_VHs_HashesCopyAndUpdate(TConnId, Tls_L2Len, TlsL2Ptr);

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, Tls_L2Len, TLS_REC_CONTENT_HS))
  {
    TLS_LOG_HS_DIV("Tls_VHs_SendClientCertVeri: client certificate was sent");

    Tls_HandShakeState[TConnId] = TLS_HS_STATE_CERT_VERI_SENT;
    return;
  }
  else
  {
    /* send failed, so revert to old hash ws (copy hash workspaces) and do not set HandshakeState to next value */
    Tls_VHs_HashesRevert(TConnId);
    return;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  Tls_VHs_AddRsaSignToClientCertVeriMsg
 **********************************************************************************************************************/
/*! \brief         Add the signature to the client_certificate_verify message.
 *  \param[in]     TConnId               Connection ID
 *  \param[out]    MsgPtr                Pointer to the message that shall be sent/written
 *  \param[in]     HashOffset            Offset of the hash in the messageConnection ID
 *  \param[in]     HashSize              Size of the hash that shall be used
 *  \param[in]     ExtraLenHashAndSign   Additional length needed for TLS1.2 hash and sign identifier
 *  \param[in]     SignInputPtr          Pointer to the array used as input for the signature calculation
 *  \param[out]    SignLenPtr            Length of the signature
 *  \return        void
 *  \note          -
 *  \context       task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_AddRsaSignToClientCertVeriMsg(
    uint8  TConnId,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_VAR) MsgPtr,
    uint16 HashOffset,
    uint16 HashSize,
    uint16 ExtraLenHashAndSign,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_VAR) SignInputPtr,
    P2VAR(uint16, AUTOMATIC, TLS_APPL_VAR) SignLenPtr /*out*/ )
{
  /* RSA signature algorithm without specified hash algorithm */
      /* RSA signature using p, q, dp, dq, qi */

      eslt_WorkSpaceRSACRTsig *RsaSignWsPtr = (eslt_WorkSpaceRSACRTsig *)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
      eslt_ErrorCode           ErrorCode    = ESL_ERC_NO_ERROR;  /* Variable is initialized to avoid compiler warnings. The variable will now always be overwritten before it is read. */
      uint16                   SignLenCalc;
      uint8                   *KeyMatPtr    = Tls_ClientKeyPtr[TConnId];
      uint16                   ModLen;
      uint8                   *KeyElementsPtr[TLS_RSA_KEY_NUM_ELEMENTS];  /* P, Q, DP, DQ, IQ */
      uint16                   KeyElementsLen[TLS_RSA_KEY_NUM_ELEMENTS];  /* P, Q, DP, DQ, IQ */
      uint16                   RsaSignLen;

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_OFF)
      TLS_UNUSED_PARAMETER(HashOffset)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
      TLS_UNUSED_PARAMETER(HashSize)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
      TLS_UNUSED_PARAMETER(ExtraLenHashAndSign)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

      /* get info from KeyMat */
      {
        CONST(uint8, TLS_CONST) MaxEleDepth        = 2;
        CONST(uint8, TLS_CONST) EleNrKeyRoot[1]    = { 1 };     /* chapter 1: root of the key material */
        CONST(uint8, TLS_CONST) EleNrKeyMod[2]     = { 1, 2 };  /* chapter 1.2: modulus */
        CONST(uint8, TLS_CONST) KeyElements[TLS_RSA_KEY_NUM_ELEMENTS][2] =
        {
          { 1, 5 },  /* chapter 1.5: key element P */
          { 1, 6 },  /* chapter 1.6: key element Q */
          { 1, 7 },  /* chapter 1.7: key element DP */
          { 1, 8 },  /* chapter 1.8: key element DQ */
          { 1, 9 }   /* chapter 1.9: key element QI */
        };

        uint8 LocIdx;
        IpBase_ReturnType           BerDecodingRes;
        IpBase_BerWorkspaceType     Workspace;
        IpBase_BerStackElementType  BerStack[2];     /* max depth of one element */
        IpBase_BerElementType       EleDesc;
        uint8                      *DataPtr = KeyMatPtr;                  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
        uint32                      DataLen = Tls_ClientKeyLen[TConnId];  /* max */

        /* Initialize the workspace (only needed once) */
        IpBase_BerInitWorkspace(&Workspace, &BerStack[0], MaxEleDepth);

        /* get overall key length */
        BerDecodingRes = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrKeyRoot[0], 1, DataPtr, DataLen);
        if( BerDecodingRes == IPBASE_E_OK )
        {
          /* now, EleDesc contains the index information */
          DataLen                   = (uint16)(EleDesc.EndIdx - EleDesc.Idx);
          Tls_ClientKeyLen[TConnId] = (uint16)DataLen;
        }
        else
        {
          return;
        }

        /* get mod length */
        BerDecodingRes = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrKeyMod[0], 2, DataPtr, DataLen);
        if( BerDecodingRes == IPBASE_E_OK )
        {
          /* now, EleDesc contains the index information */
          ModLen = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
          if((ModLen & 0x1) != 0)
          {
            ModLen--;
          }
          RsaSignLen = ModLen;
        }
        else
        {
          return;
        }

        /* get position and length for P, Q, DP, DQ, QI */
        for(LocIdx = 0; LocIdx < TLS_RSA_KEY_NUM_ELEMENTS; LocIdx++)
        {
          BerDecodingRes = IpBase_BerGetElement(&Workspace, &EleDesc, &KeyElements[LocIdx][0], 2, DataPtr, DataLen);
          if( BerDecodingRes == IPBASE_E_OK )
          {
            /* now, EleDesc contains the index information */
            KeyElementsLen[LocIdx] = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
            /* Check if the integer value of the element starts with a '0' due to BER coding rules. */
            if(KeyMatPtr[EleDesc.ContentIdx] == 0)
            {
              /* There is a leading '0', so skip the first byte and reduce the length by one. */
              KeyElementsLen[LocIdx]--;
              KeyElementsPtr[LocIdx] = &KeyMatPtr[EleDesc.ContentIdx +1];
            }
            else
            {
              /* No leading '0', so take the element as it is. */
              KeyElementsPtr[LocIdx] = &KeyMatPtr[EleDesc.ContentIdx];
            }
          }
          else
          {
            return;
          }
        }
      }

          /* init workspace */
          /* The return value of esl_initWorkSpaceHeader() can always be ignored since it can not fail when used with proper parameters */
          (void)esl_initWorkSpaceHeader(&RsaSignWsPtr->header, ESL_MAXSIZEOF_WS_RSA_CRT_SIG, NULL_PTR);  /* PRQA S 3393,3395 */ /* MD_TLS_12.1_3393_3395 */ /*lint !e506 */ /* "Constant value Boolean" (only in some configuration variants) */

          /* calc complete signature */
          {
            *SignLenPtr = RsaSignLen;
            SignLenCalc = ModLen;

            switch(Tls_ProtVer[TConnId])
            {
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
            case TLS_PROT_VER_TLS12:
            {
              /* ExtraLenHashAndSign */
              MsgPtr[4] = TLS_HASH_ALGORITHM_SHA256;  /* decision: allways use RSA with SHA256 */
              MsgPtr[5] = TLS_SIGN_ALGORITHM_RSA;
              IPBASE_PUT_UINT16(MsgPtr, 6, RsaSignLen);  /* RSA signature size */

              /* For the signature some signature description and the hash of the field that shall be signed are needed */
              /* SHA-256: (0x)30 31 30 0d 06 09 60 86 48 01 65 03 04 02 01 05 00 04 20 || H. (32byte)
              --> TLS_CERT_VERI_RSA_SHA256_SIGN_DESCR_LEN + TLS_HASH_SHA256_LEN = 51 byte total size */
              SignInputPtr[0]  = 0x30;  /* type = set */
              SignInputPtr[1]  = 0x31;  /* len (OID+extension) + hash */
              SignInputPtr[2]  = 0x30;  /* type = set */
              SignInputPtr[3]  = 0x0D;  /* len OID + extension */
              SignInputPtr[4]  = 0x06;  /* type = OID */
              SignInputPtr[5]  = 0x09;  /* length of OID */
              SignInputPtr[6]  = 0x60;  /* OID */
              SignInputPtr[7]  = 0x86;
              SignInputPtr[8]  = 0x48;
              SignInputPtr[9]  = 0x01;
              SignInputPtr[10] = 0x65;
              SignInputPtr[11] = 0x03;
              SignInputPtr[12] = 0x04;
              SignInputPtr[13] = 0x02;
              SignInputPtr[14] = 0x01;
              SignInputPtr[15] = 0x05;  /* extension */
              SignInputPtr[16] = 0x00;  /* length of extension -> no extension */
              SignInputPtr[17] = 0x04;  /* type = octet string */
              SignInputPtr[18] = 0x20;  /* length of octet string (here: sha256 hash) */

              /* generate the signature */
              ErrorCode = Crypto_SignRSACRTgen_V15( RsaSignWsPtr, KeyElementsLen[0], KeyElementsPtr[0],
                KeyElementsLen[1], KeyElementsPtr[1], KeyElementsLen[2], KeyElementsPtr[2],
                KeyElementsLen[3], KeyElementsPtr[3], KeyElementsLen[4], KeyElementsPtr[4],
                (HashOffset + HashSize), &SignInputPtr[0], &SignLenCalc, &MsgPtr[6 + ExtraLenHashAndSign] );

              break;
            }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
            case TLS_PROT_VER_TLS10:
            {
              IPBASE_PUT_UINT16(MsgPtr, 4, RsaSignLen);  /* RSA signature size */

              /* generate the signature */
              ErrorCode = Crypto_SignRSACRTgen_V15( RsaSignWsPtr, KeyElementsLen[0], KeyElementsPtr[0],
                KeyElementsLen[1], KeyElementsPtr[1], KeyElementsLen[2], KeyElementsPtr[2],
                KeyElementsLen[3], KeyElementsPtr[3], KeyElementsLen[4], KeyElementsPtr[4],
                (uint16)(Tls_HsHashLen[TConnId]), &SignInputPtr[0], &SignLenCalc, &MsgPtr[6] );

              break;
            }
#endif
            default:
              {
                /* one of the above elements will always be true -> do nothing here */
              }
            }

          }

      if((ESL_ERC_NO_ERROR != ErrorCode) || (ModLen != SignLenCalc))  /* PRQA S 3356 */ /* MD_TLS_13.7_3356 */
      {
        /* error */
        return;
      }
      TLS_LOG_HS_DIV_1("Tls_VHs_SendClientCertVeri: rsa signature length is %d byte", SignLenCalc);
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */

#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
STATIC FUNC(void, TLS_CODE) Tls_VHs_AddEcdsaSignToClientCertVeriMsg(
    uint8  TConnId,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_VAR) MsgPtr,
    uint16 ExtraLenHashAndSign,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_VAR) SignInputPtr,
    P2VAR(uint16, AUTOMATIC, TLS_APPL_VAR) SignLenPtr /*out*/ )
{
  uint8 *KeyPtr;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
  uint16 KeyLen;
  uint8 LocPrivKey[32];

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_OFF)
  TLS_UNUSED_PARAMETER(ExtraLenHashAndSign)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  /* read priv key info from static client key */
  {
    CONST(uint8, TLS_CONST) EleNrPrivKey[2]= { 1, 2 };     /* chapter 1.2  : ecc priv key */
    CONST(uint8, TLS_CONST) MaxEleDepth    = 2;

    IpBase_BerWorkspaceType     Workspace;
    IpBase_BerStackElementType  BerStack[2];  /* max depth of one element */
    IpBase_BerElementType       EleDesc;

    uint8 *DataPtr = Tls_ClientKeyPtr[TConnId]; /* information given at Tls_SetClientCertInfo() */  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
    uint32 DataLen = Tls_ClientKeyLen[TConnId]; /* max */

    /* initialize BER decoder workspace */
    IpBase_BerInitWorkspace(&Workspace, &BerStack[0], MaxEleDepth); /* only needed once */

    /* get priv key info */
    {
      if(IPBASE_E_OK != IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrPrivKey[0], 2, DataPtr, DataLen))
      {
        return;
      }
      /* now, EleDesc contains the index information */
      KeyLen = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      KeyPtr = &DataPtr[EleDesc.ContentIdx];

      /* only prime256v1 curve is supported, so key len must always be 0x20 */
      if(0x20 != KeyLen)
      {
        if(0x20 < KeyLen)
        {
          /* key in the struct is bigger than needed, so remove / ignore the leeding fill-bytes */
          KeyPtr = &KeyPtr[KeyLen - 0x20];
          KeyLen = 0x20;
        }
        else
        {
          /* key in the struct is smaller than needed, so fill-bytes are needed */
          uint8 SizeDiff = 0x20 - (uint8)KeyLen;
          uint8 CopyIndex;
          /* fill with leading '0' */
          for(CopyIndex=0; CopyIndex < SizeDiff; CopyIndex++)
          {
            LocPrivKey[CopyIndex] = 0x00;
          }
          /* copy key to local array */
          for(CopyIndex=0; CopyIndex < (uint8)KeyLen; CopyIndex++)
          {
            LocPrivKey[CopyIndex + SizeDiff] = KeyPtr[CopyIndex];
          }
          KeyPtr = &LocPrivKey[0];
          KeyLen = 0x20;
        }
      }
    }
  }

  /* sign hash of previous handshake messages, use static key from client certificate / priv key */
  {
    eslt_WorkSpaceEcP        *EcWorkSpPtr     = (eslt_WorkSpaceEcP *)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
    const eslt_EccDomain     *EcDomainPtr     = &Crypto_EccCurveANSIp256r1Domain[0];
    uint16 SignCompLen;  /* 32 byte in case of p256v1 */
    uint8 *HashPtr;
    uint8  HashLen;
    uint8 *SignatureValuePtr;
    uint16 SignTotalLen;

    SignCompLen       = esl_getLengthOfEcPsignature_comp(EcDomainPtr);
    SignatureValuePtr = &MsgPtr[6];
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    if(TLS_PROT_VER_TLS12 == Tls_ProtVer[TConnId])
    {
      /* there are two additional bytes used in TLS 1.2, so inc the pointer here */
      SignatureValuePtr = &SignatureValuePtr[2];
    }
#endif
    if(TLS_HS_HASH_ALG_MD5SHA1 == Tls_HsHashAlg[TConnId])
    {
      HashPtr    = &SignInputPtr[TLS_HASH_MD5_LEN];
      HashLen    = TLS_HASH_SHA1_LEN;
    }
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    else if(TLS_HS_HASH_ALG_SHA256 == Tls_HsHashAlg[TConnId])
    {
      HashPtr    = &SignInputPtr[0 +2];
      HashLen    = TLS_HASH_SHA256_LEN;

      /* set the two bytes introduced in TLS1.2 containing information about the used hash-signature combination */
      MsgPtr[TLS_HS_C_CERT_VERI_OFFSET_HASH_ALG] = TLS_HASH_ALGORITHM_SHA256;
      MsgPtr[TLS_HS_C_CERT_VERI_OFFSET_SIGN_ALG] = TLS_SIGN_ALGORITHM_ECDSA;
    }
#endif
    else
    {
      /* invalid hash alg */
      /* This can never happen, since supported hash algorithms are checked earlier */
      return;  /* error */
    }

    /* Crypto_GenerateEcdsaSignature can only fail for invalid input parameters or if the used random generator does not provide random data, so the return value can be ignored here */
    (void)Crypto_GenerateEcdsaSignature(EcWorkSpPtr, SignatureValuePtr, KeyPtr, HashPtr, &SignCompLen, KeyLen, HashLen, TRUE);

    SignTotalLen = SignCompLen;

    /* write ECDSA signature length to field */
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    IPBASE_PUT_UINT16(MsgPtr, (TLS_HS_C_CERT_VERI_TLS10_OFFSET_SIGN_TOT_LEN + ExtraLenHashAndSign), SignTotalLen);
#else
    IPBASE_PUT_UINT16(MsgPtr, TLS_HS_C_CERT_VERI_TLS10_OFFSET_SIGN_TOT_LEN, SignTotalLen);
#endif
    *SignLenPtr = (SignTotalLen);

  }

  /* calc master secret, using the _ephemeral_ key of the client */
  {
    Std_ReturnType Ret;

    KeyPtr = &Tls_PrivEccKeyArray[TConnId * 32];
    Ret = Tls_VHs_CalcEcdhMasterSecret(TConnId, KeyPtr);
    if(E_OK != Ret)
    {
      /* under normal conditions this error should never happen, since only a secret generation and HMAC calcualtions
      are executed, without any random input or other possibly not available elements */
#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
      TLS_CallDetReportError(TLS_API_ID_V_HS_SEND_CLIENT_CERT_VERI, TLS_E_UNEXPECTED_CRYPTO_ERROR)
#endif
        return;  /* error */
    }
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif  /* (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON) */
#endif  /* (TLS_SUPPORT_CLIENT_CERT == STD_ON) */

/***********************************************************************************************************************
 *  Tls_VCcs_SendChangeCipherSpec
 **********************************************************************************************************************/
/*! \brief      Send a client_change_cipher_spec
 *  \param[in]  TConnId        TLS connection id
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VCcs_SendChangeCipherSpec(
    uint8 TConnId )
{
  uint8 *TlsL2Ptr;
  uint16 Tls_L2Len;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnVoid((TLS_COND_INIT), TLS_API_ID_V_CCS_SEND_CHANGE_CIPHER_SPEC, TLS_E_NOT_INITIALIZED)
  TLS_CheckDetErrorReturnVoid((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_CCS_SEND_CHANGE_CIPHER_SPEC, TLS_E_INV_PARAM)
#endif

  /* request internal buffer, fill with message, then forward data to RECORD_LAYER */

  /* CLIENT CHANGE_CIPHER_SPEC -------------------------------------------------------------- */

  /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  /* change cipher spec message has only one byte, and its value is '1' */
  TlsL2Ptr[0] = 1;

  Tls_L2Len = 1;

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, Tls_L2Len, TLS_REC_CONTENT_CCS))  /* RL will prepend its header separately */
  {
    Tls_SecurityParametersType  *Temp_SecParamPtr;

    TLS_LOG_HS_DIV("Tls_VCcs_SendChangeCipherSpec: client change cipher spec was sent");

    /* calculate new keys */
    switch(Tls_SecParamTxPassivePtr[TConnId]->bulk_cipher_algorithm)
    {
    case(TLS_BULK_CIPHER_ALGORITHM_AES128_CBC):
      {
        /* X_WITH_AES_128_CBC_SHAX
           TLS_RSA_WITH_AES_128_CBC_SHA
           TLS_RSA_WITH_AES_128_CBC_SHA256
           TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA
           TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
           TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
           TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
        */
        uint8 MacLen;
        uint8 KeyBlockSize;
        uint8 KeyOffset;
        uint8 IvOffset;
        uint8 KeyBlock[TLS_KEY_BLOCK_SIZE_AES128_SHA256];  /* supported HMAC sizes up to 32 bytes (sha256) */

        if(TLS_HASH_SHA256_LEN == Tls_SecParamTxPassivePtr[TConnId]->mac_length)
        {
          MacLen       = TLS_HASH_SHA256_LEN;
          KeyBlockSize = TLS_KEY_BLOCK_SIZE_AES128_SHA256;
          KeyOffset    = TLS_KEY_PART_OFFSET_AES128_SHA256_KEY;
          IvOffset     = TLS_KEY_PART_OFFSET_AES128_SHA256_IV;
        }
        else
        {
          MacLen       = TLS_HASH_SHA1_LEN;
          KeyBlockSize = TLS_KEY_BLOCK_SIZE_AES128_SHA1;
          KeyOffset    = TLS_KEY_PART_OFFSET_AES128_SHA1_KEY;
          IvOffset     = TLS_KEY_PART_OFFSET_AES128_SHA1_IV;
        }

        if(E_OK == Tls_VCalcKeyBlock(TConnId, &KeyBlock[0], KeyBlockSize))
        {
          /* copy mac, key, and iv (for tx and rx) */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[TLS_KEY_PART_OFFSET_ALL_MAC])), (const IpBase_CopyDataType*)(&KeyBlock[ 0]), MacLen);  /* MAC */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[KeyOffset])), (const IpBase_CopyDataType*)(&KeyBlock[2*KeyOffset]), TLS_AES128_KEY_LEN);  /* KEY */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[IvOffset ])), (const IpBase_CopyDataType*)(&KeyBlock[2*IvOffset ]), TLS_AES128_KEY_LEN);  /* IV */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockRxPassivePtr[TConnId]))[TLS_KEY_PART_OFFSET_ALL_MAC])), (const IpBase_CopyDataType*)(&KeyBlock[0+MacLen]), MacLen);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockRxPassivePtr[TConnId]))[KeyOffset])), (const IpBase_CopyDataType*)(&KeyBlock[(2*KeyOffset)+TLS_AES128_KEY_LEN]), TLS_AES128_KEY_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockRxPassivePtr[TConnId]))[IvOffset ])), (const IpBase_CopyDataType*)(&KeyBlock[(2*IvOffset)+TLS_AES128_KEY_LEN]), TLS_AES128_KEY_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
        }
        Tls_TxCipherTextSizes[TConnId] = ((Tls_VCfgGetTxCipherTextSizes()[TConnId]) / TLS_AES_BLOCK_LEN) * TLS_AES_BLOCK_LEN;
        Tls_TxPlainTextSizes[TConnId]  = Tls_TxCipherTextSizes[TConnId] - (MacLen + TLS_RL_HDR_LEN + 1);  /* X - MAC - Header - Padding */
        break;
      }
    case(TLS_BULK_CIPHER_ALGORITHM_NULL):
      {
        /* X_WITH_NULL_SHAX
           TLS_RSA_WITH_NULL_SHA
           TLS_RSA_WITH_NULL_SHA256
           TLS_ECDH_ECDSA_WITH_NULL_SHA */
        uint8 MacLen;
        uint8 KeyBlockSize;
        uint8 KeyBlock[TLS_KEY_BLOCK_SIZE_NULL_SHA256];  /* supported HMAC sizes up to 32 bytes (sha256) */

        if(TLS_HASH_SHA256_LEN == Tls_SecParamTxPassivePtr[TConnId]->mac_length)
        {
          MacLen       = TLS_HASH_SHA256_LEN;
          KeyBlockSize = TLS_KEY_BLOCK_SIZE_NULL_SHA256;
        }
        else
        {
          MacLen       = TLS_HASH_SHA1_LEN;
          KeyBlockSize = TLS_KEY_BLOCK_SIZE_NULL_SHA1;
        }

        if(E_OK == Tls_VCalcKeyBlock(TConnId, &KeyBlock[0], KeyBlockSize))
        {
          IpBase_Copy((IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[TLS_KEY_PART_OFFSET_ALL_MAC])), (const IpBase_CopyDataType*)(&KeyBlock[ 0]), MacLen);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy((IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockRxPassivePtr[TConnId]))[TLS_KEY_PART_OFFSET_ALL_MAC])), (const IpBase_CopyDataType*)(&KeyBlock[MacLen]), MacLen);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          TLS_LOG_HS_DIV_1("Tls_VCcs_SendChangeCipherSpec: keyblock client (mac): 0x%X", KeyBlock[ 0]);
          TLS_LOG_HS_DIV_1("Tls_VCcs_SendChangeCipherSpec: keyblock server (mac): 0x%X", KeyBlock[MacLen]);
        }
        Tls_TxCipherTextSizes[TConnId] = Tls_VCfgGetTxCipherTextSizes()[TConnId];
        Tls_TxPlainTextSizes[TConnId]  = Tls_TxCipherTextSizes[TConnId] - MacLen;  /* X - MAC */
        break;
      }
#if (TLS_SUPPORT_AES_GCM == STD_ON)
    case(TLS_BULK_CIPHER_ALGORITHM_AES128_GCM):
      {
        /* X_WITH_AES_128_GCM_SHAX
           TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
           TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 */
        uint8 KeyBlockSize;
        uint8 KeyOffset;
        uint8 KeyLen;
        uint8 IvOffset;
        uint8 IvLen;
        uint8 KeyBlock[TLS_KEY_BLOCK_SIZE_AES128_GCM];

        KeyLen       = TLS_AES128_KEY_LEN;
        IvLen        = TLS_AES_GCM_FIXED_IV_LENGTH;
        KeyBlockSize = TLS_KEY_BLOCK_SIZE_AES128_GCM;
        KeyOffset    = TLS_KEY_PART_OFFSET_AES128_GCM_KEY;
        IvOffset     = TLS_AES128_KEY_LEN;

        if(E_OK == Tls_VCalcKeyBlock(TConnId, &KeyBlock[0], KeyBlockSize))
        {
          /* copy key, and iv (for tx and rx) */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[KeyOffset])), (const IpBase_CopyDataType*)(&KeyBlock[0]), KeyLen);           /* KEY */  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[IvOffset ])), (const IpBase_CopyDataType*)(&KeyBlock[2*IvOffset ]), IvLen);  /* IV */   /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockRxPassivePtr[TConnId]))[KeyOffset])), (const IpBase_CopyDataType*)(&KeyBlock[0+KeyLen]), KeyLen);           /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
          IpBase_Copy( (IpBase_CopyDataType*)(&(((uint8*)(Tls_KeyMatBlockRxPassivePtr[TConnId]))[IvOffset ])), (const IpBase_CopyDataType*)(&KeyBlock[(2*IvOffset)+IvLen]), IvLen);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */

          /* set pointers to keys and ivs */
          Tls_SecParamTxPassivePtr[TConnId]->KeyPtr = &(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[KeyOffset]);         /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
          Tls_SecParamTxPassivePtr[TConnId]->IvPtr  = &(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[IvOffset]);          /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
          Tls_SecParamRxPassivePtr[TConnId]->KeyPtr = &(((uint8*)(Tls_KeyMatBlockRxPassivePtr[TConnId]))[KeyOffset]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
          Tls_SecParamRxPassivePtr[TConnId]->IvPtr  = &(((uint8*)(Tls_KeyMatBlockRxPassivePtr[TConnId]))[IvOffset]);   /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
        }
        Tls_TxCipherTextSizes[TConnId] = Tls_VCfgGetTxCipherTextSizes()[TConnId];
        Tls_TxPlainTextSizes[TConnId]  = (uint16)(Tls_TxCipherTextSizes[TConnId] - TLS_AES_GCM_AD_EXTRA_LEN);
        break;
      }
#endif
    default:
      {
        /* no valid bulk cipher algorithm selected */
        /* this error case should be handled earlier in server_hello */
        return;  /* error */
      }
    }

    /* switch active and passive parameters for tx */
    Temp_SecParamPtr                  = Tls_SecParamTxPassivePtr[TConnId];
    Tls_SecParamTxPassivePtr[TConnId] = Tls_SecParamTxActivePtr[TConnId];
    Tls_SecParamTxActivePtr[TConnId]  = Temp_SecParamPtr;

    /* reset sequence number */
    Tls_VSeqNumInit(&Tls_TxSeqNum[TConnId]);

    /* update handshake state */
    Tls_HandShakeState[TConnId] = TLS_HS_STATE_CCS_SENT;

    return;  /* OK */
  }
  else
  {
    return;  /* NOT OK */
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  Tls_VHs_SendClientFinished
 **********************************************************************************************************************/
/*! \brief      continue handshake, send a client_finished msg
 *  \param[in]  TConnId        TLS connection id
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_SendClientFinished(
    uint8 TConnId )
{
  uint8 *TlsL2Ptr;
  uint16 Tls_L2Len;
  uint8 Hash_Md5AndSha1[TLS_HASH_MD5_LEN+TLS_HASH_SHA1_LEN];  /* this size (36byte) also fits for SHA256 (32byte) */

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnVoid((TLS_COND_INIT), TLS_API_ID_V_HS_SEND_CLIENT_FINISHED, TLS_E_NOT_INITIALIZED)
  TLS_CheckDetErrorReturnVoid((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_HS_SEND_CLIENT_FINISHED, TLS_E_INV_PARAM)
#endif

  /* request internal buffer, fill with handshake message, then forward data to RECORD_LAYER */

  /* CLIENT FINISHED -------------------------------------------------------------- */

  /* generate MD5 and SHA hash over all HS messages sent and received by the client so far
     send 12 byte verify_data */
  /* verify_data = PRF(master_secret, finished_label, MD5(handshake_messages) ++ SHA-1(handshake_messages), len=12); */

  /* finalize hash of HS messages */
  /* hash2 is a copy of hash1. hash2 will be closed here, hash1 will be contiued. */
  switch(Tls_HsHashAlg[TConnId])
  {
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  case TLS_HS_HASH_ALG_MD5SHA1:
    {
      /* calculate hash with MD5 and SHA1, and put both into one array */
      eslt_ErrorCode ErrorCode;

      /* MD5 finalization */
      IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        (const IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        sizeof(Tls_WorkSpaceMd5_Hs1[TConnId]) );
      ErrorCode = esl_finalizeMD5( &Tls_WorkSpaceMd5_Hs2[TConnId], &Hash_Md5AndSha1[0] );
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return;
      }

      /* SHA1 finalization */
      IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        sizeof(eslt_WorkSpaceSHA1) );
      ErrorCode = esl_finalizeSHA1( (eslt_WorkSpaceSHA1*)&Tls_WorkSpaceShaX_Hs2[TConnId], &Hash_Md5AndSha1[TLS_HASH_MD5_LEN] );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return;
      }
      break;
    }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  case TLS_HS_HASH_ALG_SHA256:
    {
      /* Here we need the sha256 hash or a hash especially defined by the cipher suite in TLS 1.2 */
      /* calculate hash with SHA256 */
      eslt_ErrorCode ErrorCode;

      /* SHA256 finalization */
      IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
        sizeof(eslt_WorkSpaceSHA256) );
      ErrorCode = esl_finalizeSHA256( (eslt_WorkSpaceSHA256*)&Tls_WorkSpaceShaX_Hs2[TConnId], &Hash_Md5AndSha1[0] );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return;
      }
      break;
    }
#endif
  default:
    {
      /* error, inv alg */
      return;
    }
  }

  /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  /* handshake type: (1 byte) */
  TlsL2Ptr[0] = TLS_HS_TYPE_FINISHED;

  {
    uint8 *L_TgtPtr    = &TlsL2Ptr[4];
    const uint8 *L_SecretPtr = &(Tls_SecParamTxActivePtr[TConnId]->master_secret[0]);
    const uint8 *L_LabelPtr  = (const uint8 *)&Tls_Hs_ClientFinishedLabel[0];
    const uint8 *L_SeedPtr   = &Hash_Md5AndSha1[0];

    if(E_OK != Tls_VPrf( TConnId,
                         L_TgtPtr, L_SecretPtr, L_LabelPtr, L_SeedPtr,
                         TLS_HS_X_FINISHED_LEN, TLS_MASTER_SECRET_LEN, TLS_LABEL_CLIENT_FINISHED_LEN, Tls_HsHashLen[TConnId]) )
    {
      return;  /* error */
    }
  }
  Tls_L2Len = TLS_HS_X_FINISHED_LEN + TLS_HS_HDR_LEN;

  /* length: (3 byte) */
  TlsL2Ptr[1] = 0;
  TlsL2Ptr[2] = 0;
  TlsL2Ptr[3] = TLS_HS_X_FINISHED_LEN;  /* L2 length without msg type and this length field. Length is small enough to fit into 1 byte. */

  /* copy and update hash workspaces */
  Tls_VHs_HashesCopyAndUpdate(TConnId, Tls_L2Len, TlsL2Ptr);

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, Tls_L2Len, TLS_REC_CONTENT_HS))
  {
    TLS_LOG_HS_DIV("Tls_VHs_SendClientFinished: client finished was sent");
    Tls_HandShakeState[TConnId] = TLS_HS_STATE_FINISHED_SENT;
    return;
  }
  else
  {
    /* send failed, so revert to old hash ws (copy hash workspaces) and do not set HandshakeState to next value */
    Tls_VHs_HashesRevert(TConnId);
    return;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
/***********************************************************************************************************************
 *  Tls_VHb_SendHeartbeatRequest
 **********************************************************************************************************************/
/*! \brief      Send a heartbeat request message
 *  \param[in]  TConnId        TLS connection id
 *  \context    task level (Tls_MainFunction)
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHb_SendHeartbeatRequest(
    uint8 TConnId )
{
  /*
    RFC 6520: TLS/DTLS Heartbeat Extension
    format:
    - HeartbeatMode: peer_allowed_to_send(1), peer_not_allowed_to_send(2)
    - HeartbeatMessageType: heartbeat_request(1), heartbeat_response(2)
    - Hello Extension: heartbeat extension type (15)
    - Protocol: heartbeat content type (24)
   
    struct {
        HeartbeatMessageType type;
        uint16 payload_length;
        opaque payload[HeartbeatMessage.payload_length];
        opaque padding[padding_length];
    } HeartbeatMessage;
   
    The total length of a HeartbeatMessage MUST NOT exceed 2^14 or max_fragment_length when negotiated as defined in [RFC6066].
  */

  uint8        *TlsL2Ptr;
  uint16        Tls_L2Len;
  uint32        CurrentPos;
  uint8_least   paddingByteIdx;

    /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  TlsL2Ptr[TLS_HB_OFFSET_MSG_TYPE] = TLS_HB_MSG_HEARTBEAT_REQUEST;  /* handshake type: (1 byte) */
  IPBASE_PUT_UINT16(TlsL2Ptr, TLS_HB_OFFSET_PAYLOAD_LEN, (Tls_HeartbeatRequestStringTextLen + Tls_HeartbeatCounterStringLen));  /*lint !e572 */
  CurrentPos                       = TLS_HB_OFFSET_PAYLOAD;

  /* insert some payload to the heartbeat, the content is not defined by RFC but can be any value */
  IpBase_Copy(&TlsL2Ptr[CurrentPos], &Tls_HeartbeatRequestString[0], Tls_HeartbeatRequestStringTextLen);
  CurrentPos += Tls_HeartbeatRequestStringTextLen;
  IPBASE_PUT_UINT32(TlsL2Ptr, CurrentPos, Tls_HeartbeatCounter[TConnId]);
  Tls_HeartbeatCounter[TConnId]++;
  CurrentPos += Tls_HeartbeatCounterStringLen;

  /* append some 'random' padding, at least 16 bytes (according to RFC6520) */
  for(paddingByteIdx = 0; paddingByteIdx < TLS_HEARTBEAT_MIN_PADDING_SIZE; paddingByteIdx++)
  {
    TlsL2Ptr[CurrentPos + paddingByteIdx] = (uint8)paddingByteIdx;
  }
  CurrentPos += TLS_HEARTBEAT_MIN_PADDING_SIZE;

  Tls_L2Len = (uint16)CurrentPos;

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, Tls_L2Len, TLS_REC_CONTENT_HB))  /* RL will prepend its header separately */
  {
    /* ok */
  }
}

/***********************************************************************************************************************
 *  Tls_VHb_Rx
 **********************************************************************************************************************/
/*! \brief      Receive heartbeat message
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \param[in]  RxLen          data length that shall be received
 *  \return                    consumed length, (part of) RL header length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHb_Rx(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen )
{
  uint16 RetLen = RxLen;
  uint8  MessageType;

  TLS_LOG_DIV("Tls_VHb_Rx: Heartbeat message received ! ------------------")

  /* check rx length */
  if(RxLen < (3 + TLS_HEARTBEAT_MIN_PADDING_SIZE))  /* type, length, padding */
  {
    /* error, this is no complete heartbeat message */
    RetLen = 0;
  }

  /* read message type */
  MessageType = DataPtr[0];
  if(MessageType != TLS_HB_MSG_HEARTBEAT_RESPONSE)
  {
    /* error, we currently only support heartbeat responses */
    /* drop message silently */
  }
  else
  {
    /* check payload to be the same as sent in the heartbeat request */
    /* RFC 6520: If a received HeartbeatResponse message does not contain the expected payload, the message MUST be discarded silently. */
    uint8 CounterString[4];

    IPBASE_PUT_UINT32(&CounterString[0], 0, (Tls_HeartbeatCounter[TConnId] -1));
    if( (IPBASE_CMP_EQUAL == IpBase_StrCmpLen(&Tls_HeartbeatRequestString[0], &DataPtr[3], Tls_HeartbeatRequestStringTextLen)) &&  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
        (IPBASE_CMP_EQUAL == IpBase_StrCmpLen(&CounterString[0], &DataPtr[3 + Tls_HeartbeatRequestStringTextLen], Tls_HeartbeatCounterStringLen)) )
    {
      /* received expected message payload */

      /* ignore padding */

      /* set heartbeat status to 'communication is active' */
      Tls_HeartbeatStatus[TConnId] = TLS_HEARTBEAT_STATUS_COM_ACTIV;
    }
    else
    {
      /* message content is not as expected -> drop message silently */
    }
  }

  /* return consumed length */
  return RetLen;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (TLS_SUPPORT_HEARTBEAT == STD_ON) */

/***********************************************************************************************************************
 *  Tls_VHs_HashesCopyAndUpdate
 **********************************************************************************************************************/
/*! \brief      backup and update the continous hashes workspaces
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataLen        length of data to be hashed
 *  \param[in]  DataPtr        pointer to data to be hashed
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_HashesCopyAndUpdate(
    uint8 TConnId,
    uint16 DataLen,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr)
{
  switch(Tls_HsHashAlg[TConnId])
  {
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  case TLS_HS_HASH_ALG_MD5SHA1:
    {
      /* copy hashes (md5 + sha1) */
      (void)IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         (const IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         sizeof(Tls_WorkSpaceMd5_Hs1[TConnId]) );
      (void)IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         sizeof(eslt_WorkSpaceSHA1) );
      break;
    }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  case TLS_HS_HASH_ALG_SHA256:
    {
      /* copy hash (sha256) */
      (void)IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         sizeof(eslt_WorkSpaceSHA256) );
      break;
    }
#endif
  default:
    {
      /* error, invalid type */
      return;
    }
  }

  /* update hashes */
  Tls_VHs_HashMsg(TConnId, DataPtr, DataLen);
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VHs_HashesRevert
 **********************************************************************************************************************/
/*! \brief      revert the continous hashes workspaces
 *  \param[in]  TConnId        TLS connection id
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_HashesRevert(
    uint8 TConnId )
{
  switch(Tls_HsHashAlg[TConnId])
  {
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  case TLS_HS_HASH_ALG_MD5SHA1:
    {
      /* copy hashes MD5 and SHA1 */
      (void)IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         (const IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         sizeof(Tls_WorkSpaceMd5_Hs1[TConnId]) );
      (void)IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         sizeof(eslt_WorkSpaceSHA1) );
      break;
    }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  case TLS_HS_HASH_ALG_SHA256:
    {
      /* copy hashes SHA256 */
      (void)IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                         sizeof(eslt_WorkSpaceSHA256) );
      break;
    }
#endif
  default:
    {
      /* error, invalid hash type */
    }
  }
}

/***********************************************************************************************************************
 *  Tls_VHs_TriggerAlertAndFin
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_TriggerAlertAndFin(
    uint8 TConnId,
    uint8 AlertLevel,
    uint8 AlertId )
{
  /* # Increase the alert level if necessary */
  if(Tls_PendingAlertLevel[TConnId] < AlertLevel)  /* TLS_ALERT_LEVEL_WARNING */
  {
    Tls_PendingAlertLevel[TConnId] = AlertLevel;
  }
  if(Tls_PendingAlertId[TConnId] == TLS_ALERT_DESC_NONE)
  {
    Tls_PendingAlertId[TConnId]    = AlertId;
  }

  /* trigger sending of the alert and then close the connection */
  if(E_OK == Tls_VAl_SendAlert(TConnId, Tls_PendingAlertLevel[TConnId], Tls_PendingAlertId[TConnId]))
  {
    if(Tls_PendingAlertLevel[TConnId] == TLS_ALERT_LEVEL_FATAL)
    {
      /* trigger closing the TCP connection */
      if(E_OK == Tls_TcpClose(Tls_VCfgGetTlsConn2SockIdMap()[TConnId]))
      {
        Tls_ConnState[TConnId]      = TLS_STATE_TCP_CLOSING_TLS_CLOSED;
        Tls_PendingAlertId[TConnId] = TLS_ALERT_DESC_NONE;
      }
      else
      {
        Tls_ConnState[TConnId]      = TLS_STATE_TCP_ESTABL_TLS_CLOSING;
        Tls_PendingAlertId[TConnId] = TLS_ALERT_DESC_TCP_FIN;
      }
    }
    else
    {
      /* TCP is already triggered for closing, just close TLS */
      Tls_ConnState[TConnId]      = TLS_STATE_TCP_CLOSING_TLS_CLOSED;
      Tls_PendingAlertId[TConnId] = TLS_ALERT_DESC_NONE;
    }
    TLS_LOG_HS_DIV_1("Tls_VHs_TriggerAlertAndFin: connection state changed to %d", Tls_ConnState[TConnId]);
  }
}

/***********************************************************************************************************************
 *  Tls_VAl_SendAlert
 **********************************************************************************************************************/
/*! \brief      send an alert
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  AlertLevel     alert level (fatal or warning)
 *  \param[in]  AlertId        alert ID
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VAl_SendAlert(
    uint8 TConnId,
    uint8 AlertLevel,
    uint8 AlertId )
{
  uint8 *TlsL2Ptr;
  uint16 Tls_L2Len;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnValue((TLS_COND_INIT), TLS_API_ID_V_AL_SEND_ALERT, TLS_E_NOT_INITIALIZED, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_AL_SEND_ALERT, TLS_E_INV_PARAM, E_NOT_OK)
#endif

  /* data pointer for upper TLS layer: */
  TlsL2Ptr = (uint8*)(Tls_VCfgGetTxPlainTextStarts()[TConnId]);

  /* alert id and level  */
  TlsL2Ptr[0] = AlertLevel;
  TlsL2Ptr[1] = AlertId;
  Tls_L2Len   = 2;

  /* forward msg to TLS record layer (L1) */
  if(E_OK == Tls_VRl_Transmit(TConnId, TlsL2Ptr, Tls_L2Len, TLS_REC_CONTENT_A))  /* RL will prepend its header separately */
  {
    TLS_LOG_DIV_1("Tls_VAl_SendAlert: alert message was sent with id %d", AlertId);
    return E_OK;
  }
  else
  {
    return E_NOT_OK;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VRl_Transmit
 **********************************************************************************************************************/
/*! \brief      send message through record layer (compressed, authenticated and encrypted)
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        Pointer to data that has to be sent
 *  \param[in]  DataLen        Length of data
 *  \param[in]  Type           Type of message (UL protocol)
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_Transmit(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 DataLen,
    uint8 Type)
{
  uint32              SeqNumBuf[2];
  uint8               RlHdrBuf[TLS_RL_HDR_LEN];
  uint8               MacData[TLS_HASH_SHA256_LEN];  /* supported HMAC sizes up to 32 bytes (sha256) */
  Std_ReturnType TransmitWithXReturnValue = E_NOT_OK;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnValue((TLS_COND_INIT), TLS_API_ID_V_RL_TRANSMIT, TLS_E_NOT_INITIALIZED, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_V_RL_TRANSMIT, TLS_E_INV_PARAM, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(DataPtr)), TLS_API_ID_V_RL_TRANSMIT, TLS_E_INV_PARAM, E_NOT_OK)
#endif

  /* Write RecordLayer header buffer (content type, protocol version and fragment length) */
  {
    /* content type: (1 byte) (handshake, ccs, alert, appl data) */
    RlHdrBuf[TLS_RL_OFFSET_MSG_TYPE] = Type;

    /* protocol version: (2 byte) */
    RlHdrBuf[TLS_RL_OFFSET_PROT_VER] = TLS_MAJOR_VER;
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
# if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
    if(Tls_ProtVer[TConnId] == TLS_PROT_VER_TLS12)
# endif
    {
      RlHdrBuf[TLS_RL_OFFSET_PROT_VER + 1] = TLS_MINOR_VER_TLS12;
    }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
# if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    else
# endif
    {
      RlHdrBuf[TLS_RL_OFFSET_PROT_VER + 1] = TLS_MINOR_VER_TLS10;
    }
#endif

    /* fragment length: (2 byte) */
    IPBASE_PUT_UINT16(&RlHdrBuf[0], TLS_RL_OFFSET_MSG_LEN, DataLen);
  }

  /* Fill sequence number buffer. Filling the sequence number field every time improves overall speed. Only the first
     handshake (with cipher suite TLS_NULL_WITH_NULL_NULL) is slower this way. */
  SeqNumBuf[0] = IPBASE_HTON32(Tls_TxSeqNum[TConnId].UpperVal);
  SeqNumBuf[1] = IPBASE_HTON32(Tls_TxSeqNum[TConnId].LowerVal);

  if(TLS_MAC_ALGORITHM_HMAC_SHA1 == Tls_SecParamTxActivePtr[TConnId]->mac_algorithm)
  {
    /* fill MAC buffer (generate MAC) */
    /* Length of MAC with SHA1 is 20 byte */
    eslt_WorkSpaceHMACSHA1 *HmacWsPtr;

    HmacWsPtr = (eslt_WorkSpaceHMACSHA1 *)(&Tls_WorkSpaceHmac[TConnId]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */

    /* Calculating the hash should never cause an error, so functions can be casted to void. */
    (void)esl_initWorkSpaceHeader( &HmacWsPtr->header,  ESL_MAXSIZEOF_WS_HMACSHA1, NULL_PTR );  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
    (void)esl_initHashMACSHA1( HmacWsPtr, TLS_HASH_SHA1_LEN,
                               (&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[TLS_KEY_PART_OFFSET_ALL_MAC])) );  /* MAC position in key-mat is always the same ('0') */ /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
    /* Variable 'SeqNum' is always set for HMAC != 0 */
    (void)esl_updateHashMACSHA1( HmacWsPtr, TLS_RL_SEQUENCE_NUM_LEN, (const eslt_Byte*)&SeqNumBuf[0] );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_x */
    (void)esl_updateHashMACSHA1( HmacWsPtr, TLS_RL_HDR_LEN, &RlHdrBuf[0] );
    (void)esl_updateHashMACSHA1( HmacWsPtr, DataLen, DataPtr );
    (void)esl_finalizeHashMACSHA1( HmacWsPtr, &MacData[0] );
  }
  else if(TLS_MAC_ALGORITHM_HMAC_SHA256 == Tls_SecParamTxActivePtr[TConnId]->mac_algorithm)
  {
    /* fill MAC buffer (generate MAC) */
    /* Length of MAC with SHA256 is 32 byte */
    uint8 MacKeyPos;
    Crypto_HmacSha256StoreType *HmacWsPtr;

    MacKeyPos = TLS_KEY_PART_OFFSET_ALL_MAC;  /* MAC position in key-mat is always the same ('0') */
    HmacWsPtr = &Tls_WorkSpaceHmac[TConnId];

    Crypto_HmacSha256Init( HmacWsPtr, (&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[MacKeyPos])), TLS_HASH_SHA256_LEN );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
        /* Variable 'SeqNum' is always set for HMAC != 0 */
    Crypto_HmacSha256Encode( HmacWsPtr, (const uint8*)&SeqNumBuf[0], TLS_RL_SEQUENCE_NUM_LEN );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_x */
    Crypto_HmacSha256Encode( HmacWsPtr, &RlHdrBuf[0], TLS_RL_HDR_LEN );
    Crypto_HmacSha256Encode( HmacWsPtr, DataPtr, DataLen );
    Crypto_HmacSha256End( HmacWsPtr, &MacData[0] );
  }
  else
  {
    /* MAC algorithm is NULL, AEAD_SHA256 or not supported */
    /* do nothing */
  }

  /* use Tls_SecParamTxActivePtr[TConnId] to encrypt the message */
  /* -----------------------------------------------------------------------------------------------
     -----------------------------------------------------------------------------------------------
     -------------------------------------------------------------------------------------------- */
  /* Differ depending on active cipher algorithm */
  switch(Tls_SecParamTxActivePtr[TConnId]->bulk_cipher_algorithm)
  {
  case TLS_BULK_CIPHER_ALGORITHM_NULL:
    {
        /* cipher suite TLS_xx_WITH_NULL_xx 
           - TLS_RSA_WITH_NULL_SHA
           - TLS_RSA_WITH_NULL_SHA256
           - TLS_ECDH_ECDSA_WITH_NULL_SHA */

      TransmitWithXReturnValue = Tls_VRl_TransmitWithNull(TConnId, &RlHdrBuf[0], &MacData[0], DataPtr, DataLen);  /* PRQA S 3354 */ /* MD_TLS_9.1_3354_d */  /*lint !e645 */ /* MacData is always initialized when necessary */
      break;
    }
  case TLS_BULK_CIPHER_ALGORITHM_AES128_CBC:
    {
      /* cipher suites with encryption 'AES128_CBC' (cipher_suites TLS_xx_WITH_AES_128_CBC_xx)
         - case TLS_RSA_WITH_AES_128_CBC_SHA
         - case TLS_RSA_WITH_AES_128_CBC_SHA256
         - case TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA
         - case TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
         - case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
         - case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 */

      TransmitWithXReturnValue = Tls_VRl_TransmitWithAesCbc( TConnId, &RlHdrBuf[0], &MacData[0], DataPtr, DataLen);
      break;
    }
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
#if (TLS_SUPPORT_AES_GCM == STD_ON)
  /* AEAD-Ciphers are only supported by min TLS1.2 */
  /*  */
  case TLS_BULK_CIPHER_ALGORITHM_AES128_GCM:
    {
      /* cipher suites with encryption 'AES128_GCM' (cipher_suites TLS_xx_WITH_AES_128_GCM_xx)
         - TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
         - TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 */

      TransmitWithXReturnValue = Tls_VRl_TransmitWithAesGcm(TConnId, &RlHdrBuf[0], DataPtr, DataLen, Type);
      break;
    }
#endif  /* (TLS_SUPPORT_AES_GCM == STD_ON) */
#endif  /* (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON) */
  default:
    {
      /* Invalid configuration, not supported TLS suite is chosen
         -> this error condition is already checked during handshake, so nothing has to be done here */
    }
  /* -----------------------------------------------------------------------------------------------
     -----------------------------------------------------------------------------------------------
     -------------------------------------------------------------------------------------------- */
  }

  return TransmitWithXReturnValue;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  Tls_VRl_TransmitWithNull
 **********************************************************************************************************************/
/*! \brief      Send data using ciper suites xx_WITH_NULL_xx.
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  RlHdrBufPtr    Pointer to an array containing the RL header
 *  \param[in]  DataPtr        Pointer to data that has to be sent
 *  \param[in]  DataLen        Length of data
 *  \return     E_OK           Transmit succeeded
 *  \return     E_NOT_OK       Transmit failed
 *  \context    TASK
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_TransmitWithNull(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) RlHdrBufPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) MacDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 DataLen )
{
  TcpIp_SocketIdType  socketId = Tls_VCfgGetTlsConn2SockIdMap()[TConnId];
  Tls_PbufType       *pbufPtr;
  Std_ReturnType      retVal = E_NOT_OK;

  if(Tls_SecParamTxActivePtr[TConnId]->mac_algorithm == TLS_MAC_ALGORITHM_NULL)
  {
    /* cipher suite TLS_NULL_WITH_NULL_NULL */
    /* no encryption or authentication at all */
    TLS_LOG_RL("Tls_VRl_Transmit: send with TLS_NULL_WITH_NULL_NULL");

    if( BUFREQ_OK == TcpIp_ProvideTxBufferInt(socketId, NULL_PTR, &pbufPtr, ((uint32)(DataLen + TLS_RL_HDR_LEN))) )  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_sock */
    {
      /* copy record layer header */
      (void)IpBase_CopyString2PbufAt(&RlHdrBufPtr[0], TLS_RL_HDR_LEN, pbufPtr, 0);

      /* copy upper TLS layer content (header, data, ...) */
      (void)IpBase_CopyString2PbufAt(DataPtr, DataLen, pbufPtr, TLS_RL_HDR_LEN);

      /* send */
      if(E_OK == TcpIp_TransmitToInt(socketId, pbufPtr, NULL_PTR))
      {
        TLS_LOG_RL_1("Tls_VRl_Transmit: sent with TLS_NULL_WITH_NULL_NULL, total RL len was %d", (DataLen + TLS_RL_HDR_LEN));
        retVal = E_OK;
      }
      else
      {
        /* transmit failed -> free the TCP buffer */
        pbufPtr->len    = 0;
        pbufPtr->totLen = 0;
        (void)TcpIp_TransmitToInt(socketId, pbufPtr, NULL_PTR);
      }
    }
    else
    {
      /* error, buffer request failed */
    }

  }
  else  /* TLS_MAC_ALGORITHM_SHAx */
  {
    /* cipher suites xx_WITH_NULL_xx
    - TLS_RSA_WITH_NULL_SHA
    - TLS_RSA_WITH_NULL_SHA256
    - TLS_ECDH_ECDSA_WITH_NULL_SHA */
    uint16 PayloadLen;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
    uint8  MacLen;

    TLS_LOG_RL("Tls_VRl_Transmit: send with TLS_X_WITH_NULL_SHAX");

    /* determine MAC length */
    MacLen = Tls_SecParamTxActivePtr[TConnId]->mac_length;

    /* no encryption: */
    PayloadLen = DataLen + MacLen;

    /* request buffer(size = RL_Header + encrypted data), then fill and send buffer */
    if( BUFREQ_OK == TcpIp_ProvideTxBufferInt(socketId, NULL_PTR, &pbufPtr, ((uint32)(PayloadLen + TLS_RL_HDR_LEN))) )
    {

      /* only adapt length field, re-use other values (Buf[0]..Buf[2]) from encoding */
      /* fragment length: (2 byte) */
      RlHdrBufPtr[3] = (uint8)((PayloadLen >> 8) & 0xFF);
      RlHdrBufPtr[4] = (uint8)((PayloadLen >> 0) & 0xFF);

      /* copy record layer header */
      (void)IpBase_CopyString2PbufAt(&RlHdrBufPtr[0], TLS_RL_HDR_LEN, pbufPtr, 0);

      /* copy upper TLS layer content (data, MAC) */
      (void)IpBase_CopyString2PbufAt(DataPtr, DataLen, pbufPtr, TLS_RL_HDR_LEN);
      (void)IpBase_CopyString2PbufAt(&MacDataPtr[0], MacLen, pbufPtr, (TLS_RL_HDR_LEN + DataLen));  /* current cipher suite uses MAC, so it is filled earlier */ /* PRQA S 3354 */ /* MD_TLS_9.1_3354_a */

      /* send */
      if(E_OK == TcpIp_TransmitToInt(socketId, pbufPtr, NULL_PTR))
      {
        TLS_LOG_RL_1("Tls_VRl_Transmit: sent with TLS_X_WITH_NULL_SHAX, total RL len was %d", (TLS_RL_HDR_LEN + DataLen));

        Tls_VSeqNumInc(&Tls_TxSeqNum[TConnId]);
        retVal = E_OK;
      }
      else
      {
        /* transmit failed, so free the buffer */
        pbufPtr->len    = 0;
        pbufPtr->totLen = 0;
        (void)TcpIp_TransmitToInt(socketId, pbufPtr, NULL_PTR);
      }
    }
    else
    {
      /* error, buffer request failed */
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 *  Tls_VRl_TransmitWithAesCbc
 **********************************************************************************************************************/
/*! \brief      Send data using ciper suites xx_WITH_AES_128_CBC_xx.
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  RlHdrBufPtr    Pointer to an array containing the RL header
 *  \param[in]  DataPtr        Pointer to data that has to be sent
 *  \param[in]  DataLen        Length of data
 *  \return     E_OK           Transmit succeeded
 *  \return     E_NOT_OK       Transmit failed
 *  \context    TASK
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_TransmitWithAesCbc(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) RlHdrBufPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) MacDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 DataLen )
{
  TcpIp_SocketIdType  socketId = Tls_VCfgGetTlsConn2SockIdMap()[TConnId];
  Tls_PbufType       *PbufPtr;
  uint8              *ivPtr;
  Std_ReturnType      retVal = E_NOT_OK;

  uint16              OutputLen;
  uint16              PayloadLen;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
  uint16              TxLen1;
  uint16              TxLen2;
  uint8               MacLen;
  uint8               ExtraLenIv;
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  uint8               IvOffset = 0;
#endif
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  uint8               IvBlock[TLS_AES_BLOCK_LEN];
#endif
  uint8               errorId = TLS_E_NO_ERROR;

  TLS_LOG_RL("Tls_VRl_Transmit: send with cipher_suites xx_WITH_AES_128_CBC_xx");

  /* determine MAC length */
  MacLen          = Tls_SecParamTxActivePtr[TConnId]->mac_length;

  /* For TLS 1.0: Set IV offset (position in KeyMat) */
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
# if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  if(Tls_ProtVer[TConnId] == TLS_PROT_VER_TLS10)
# endif
  {
    IvOffset = MacLen + TLS_AES128_KEY_LEN;  /* TLS_KEY_PART_OFFSET_AES128_SHA1_IV or TLS_KEY_PART_OFFSET_AES128_SHA256_IV) */
  }
#endif

  /* Calc length that shall be sent via TCP and set the pointer to the IV. */
  /* TCP length: TxLen1 + TxLen2 + TLS_RL_HDR_LEN */
  TxLen1 = DataLen + MacLen;
  TxLen2 = TLS_AES_BLOCK_LEN - (TxLen1 % TLS_AES_BLOCK_LEN);

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
# if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  if(Tls_ProtVer[TConnId] == TLS_PROT_VER_TLS12)
# endif
  {
    /* TLS 1.2 */
    ivPtr =  &IvBlock[0];
    ExtraLenIv = TLS_AES_BLOCK_LEN;
  }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
# if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  else
# endif
  {
    /* TLS 1.0 */
    ivPtr = (&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[IvOffset]));  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
    ExtraLenIv = 0;
  }
#endif

  /* request buffer(size = RL_Header + encrypted data), then fill and send buffer */
  if( BUFREQ_OK == TcpIp_ProvideTxBufferInt(socketId, NULL_PTR, &PbufPtr, ((uint32)(TxLen1 + TxLen2 + TLS_RL_HDR_LEN + ExtraLenIv))) )  /* PRQA S 3347 */ /* MD_TLS_9.1_3347_a */ /*lint !e644 */ /* 'ExtraLenIv' is always initialized for valid TLS protocol versions */
  {
    Tls_ReturnType RetValAes;
    uint8         *OutputPtr;
    uint8          PaddingByte;

    /* # For TLS 1.2, fill IV-block with random data */
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    if(TLS_PROT_VER_TLS12 == Tls_ProtVer[TConnId])
    {
      (void)esl_getBytesRNG(TLS_AES_BLOCK_LEN, &IvBlock[0]);
    }
#endif

    /* # Encrypt the data. */
    OutputPtr = (uint8*)&(Tls_VCfgGetTxCipherTextStarts()[TConnId][0]);
    OutputLen = Tls_TxCipherTextSizes[TConnId];
    RetValAes   = Tls_VAesEncryptData(TConnId, ivPtr, DataPtr, DataLen, OutputPtr, &OutputLen, FALSE);  /* PRQA S 3347 */ /* MD_TLS_9.1_3347_b */ /*lint !e644 */ /* 'ExtraLenIv' is always initialized for valid TLS protocol versions */
    if(TLS_E_OK != RetValAes)
    {
      /* error, encryption failed */
      TLS_LOG_RL_1("Tls_VRl_Transmit: AES encryption error 1, error code is %d", RetValAes);
      errorId = TLS_E_UNEXPECTED_CRYPTO_ERROR;
    }
    PayloadLen = OutputLen;

    if(errorId == TLS_E_NO_ERROR)
    {
      /* # Encrypt the MAC */
      OutputPtr = (uint8*)&(Tls_VCfgGetTxCipherTextStarts()[TConnId][PayloadLen]);
      OutputLen = Tls_TxCipherTextSizes[TConnId] - PayloadLen;
      RetValAes   = Tls_VAesEncryptData(TConnId, ivPtr, &MacDataPtr[0], MacLen, OutputPtr, &OutputLen, FALSE);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */  /* PRQA S 3354 */ /* MD_TLS_9.1_3354_a */  /*lint !e645 */ /* MacData is either initialized or not used */
      if(TLS_E_OK != RetValAes)
      {
        /* error, encryption failed */
        TLS_LOG_RL_1("Tls_VRl_Transmit: AES encryption error 2, error code is %d", RetValAes);
        errorId = TLS_E_UNEXPECTED_CRYPTO_ERROR;
      }
      PayloadLen += OutputLen;
    }

    if(errorId == TLS_E_NO_ERROR)
    {
      /* # Encrypt the padding length byte */
      OutputPtr = &((uint8*)(Tls_VCfgGetTxCipherTextStarts()[TConnId]))[PayloadLen];
      OutputLen = Tls_TxCipherTextSizes[TConnId] - PayloadLen;
      PaddingByte = (uint8)((TLS_AES_BLOCK_LEN -1) - ((DataLen + MacLen) % TLS_AES_BLOCK_LEN));
      RetValAes   = Tls_VAesEncryptData(TConnId, ivPtr, &PaddingByte, 1, OutputPtr, &OutputLen, TRUE);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
      if(TLS_E_OK != RetValAes)
      {
        /* error, encryption failed */
        TLS_LOG_RL_1("Tls_VRl_Transmit: AES encryption error 3, error code is %d", RetValAes);
        errorId = TLS_E_UNEXPECTED_CRYPTO_ERROR;
      }
      PayloadLen += OutputLen;
      if(0 == PaddingByte)
      {
        PayloadLen -= TLS_AES_BLOCK_LEN;
      }
    }

    if(errorId == TLS_E_NO_ERROR)
    {
      /* only adapt length field, re-use other values (Buf[0]..Buf[2]) from encoding */
      /* fragment length: (2 byte) */
      RlHdrBufPtr[3] = (uint8)(((PayloadLen + ExtraLenIv) >> 8) & 0xFF);
      RlHdrBufPtr[4] = (uint8)(((PayloadLen + ExtraLenIv) >> 0) & 0xFF);

      /* copy record layer header */
      (void)IpBase_CopyString2PbufAt(&RlHdrBufPtr[0], TLS_RL_HDR_LEN, PbufPtr, 0);

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
      /* copy IV */
      if(TLS_PROT_VER_TLS12 == Tls_ProtVer[TConnId])
      {
        (void)IpBase_CopyString2PbufAt(&IvBlock[0], TLS_AES_BLOCK_LEN, PbufPtr, TLS_RL_HDR_LEN);  /* PRQA S 3354 */ /* MD_TLS_9.1_3354_c */  /*lint !e645 */ /* 'IvBlock' is always initzialized for TLS1.2 */
      }
#endif

      /* copy upper TLS layer content (header, data, ...) */
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
      (void)IpBase_CopyString2PbufAt( &((uint8*)(Tls_VCfgGetTxCipherTextStarts()[TConnId]))[0], PayloadLen, PbufPtr, TLS_RL_HDR_LEN + ExtraLenIv);
#else
      (void)IpBase_CopyString2PbufAt( &((uint8*)(Tls_VCfgGetTxCipherTextStarts()[TConnId]))[0], PayloadLen, PbufPtr, TLS_RL_HDR_LEN);
#endif

      /* send */
      if(E_OK == TcpIp_TransmitToInt(socketId, PbufPtr, NULL_PTR))
      {
        /* # Increase the TLS packet sequence number. */
        Tls_VSeqNumInc(&Tls_TxSeqNum[TConnId]);

        /* # For TLS 1.0, copy last encrypted AES block to the KeyMatBlock for usage as IV for the next AES encryption. */
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
        if(TLS_PROT_VER_TLS10 == Tls_ProtVer[TConnId])
        {
          uint8 *IvStoragePtr;
          uint8 *NewIvPtr;
          /* IvOffset is already set earlier */

          IvStoragePtr = &(((uint8*)Tls_KeyMatBlockTxPtr[TConnId])[IvOffset]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
          NewIvPtr = &((uint8*)(Tls_VCfgGetTxCipherTextStarts()[TConnId]))[PayloadLen - TLS_AES_BLOCK_LEN];
          IpBase_Copy((IpBase_CopyDataType*)(IvStoragePtr), (const IpBase_CopyDataType*)(NewIvPtr), TLS_AES_BLOCK_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
        }
#endif

        retVal = E_OK;
      }
      else
      {
        /* transmit failed, so free the buffer */
        PbufPtr->len    = 0;
        PbufPtr->totLen = 0;
        (void)TcpIp_TransmitToInt(socketId, PbufPtr, NULL_PTR);
      }
    }
  }
  else
  {
    /* error, no buffer could be provided */
  }

  return retVal;
}  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  Tls_VRl_TransmitWithAesGcm
 **********************************************************************************************************************/
/*! \brief      Send data using ciper suites xx_WITH_AES_128_GCM.
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  RlHdrBufPtr    Pointer to an array containing the RL header
 *  \param[in]  DataPtr        Pointer to data that has to be sent
 *  \param[in]  DataLen        Length of data
 *  \param[in]  Type           Type of message (UL protocol)
 *  \return     E_OK           Transmit succeeded
 *  \return     E_NOT_OK       Transmit failed
 *  \context    TASK
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_TransmitWithAesGcm(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) RlHdrBufPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 DataLen,
    uint8 Type)
{
  TcpIp_SocketIdType socketId = Tls_VCfgGetTlsConn2SockIdMap()[TConnId];
  Tls_PbufType    *PbufPtr;
  uint32 OutputLen;
  uint16 PayloadLen;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
  uint32 TxLenTotal;

  TLS_LOG_RL("Tls_VRl_Transmit: send with TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256 or TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256");

  /* calc length that shall be sent via TCP */
  /* length = record layer header + nonce explicit + DataLen (+ AD extra length) + TagField */
  TxLenTotal = TLS_RL_HDR_LEN + TLS_AES_GCM_RECORD_IV_LENGTH + DataLen + TLS_AES_BLOCK_LEN;
  if(Type != TLS_REC_CONTENT_HS)
  {
    TxLenTotal += TLS_AES_GCM_AD_EXTRA_LEN;
  }

  /* request buffer(size = RL_Header + encrypted data), then fill and send buffer */
  if( BUFREQ_OK == TcpIp_ProvideTxBufferInt(socketId, NULL_PTR, &PbufPtr, TxLenTotal) )  /* PRQA S 3347 */ /* MD_TLS_9.1_3347_a */ /*lint !e644 */ /* 'ExtraLenIv' is always initialized for valid TLS protocol versions */
  {
    /* encrypt data using AES128_GCM_SHA256 */
    uint8  *OutputPtr;

    eslt_WorkSpaceGCM *TempWsPtr = (eslt_WorkSpaceGCM*)&Tls_WorkSpaceAes[TConnId];
    eslt_ErrorCode    ErrorCode;

    uint8 TempIv[TLS_AES_GCM_TOTAL_IV_LENGTH];  /* 4 byte salt + 8  byte nonce_explicit (counter) */
    uint8 *AesGcmKeyPtr;
    uint8 *AesGcmIvPtr = &TempIv[0];

    uint8  TempAuthData[TLS_AES_GCM_AUTH_DATA_LEN];  /* additional_data = seq_num (8) + TLSCompressed.type (1) + TLSCompressed.version (2) + TLSCompressed.length (2) */
    uint8  TagField[16];
    uint16 AuthDataLenValue;
    uint32 WriteOffset;
    uint8 *KeyMatIvPtr;

    KeyMatIvPtr   = Tls_SecParamTxActivePtr[TConnId]->IvPtr;
    AesGcmKeyPtr  = Tls_SecParamTxActivePtr[TConnId]->KeyPtr;

    /* Fill IV / NONCE (salt + nonce_explicit) */
    IpBase_Copy((IpBase_CopyDataType*)(&TempIv[0]), ((IpBase_CopyDataType*)(KeyMatIvPtr)), 4);  /* copy IV from keymat */
    IPBASE_PUT_UINT32(&TempIv[0], 4, Tls_TxSeqNum[TConnId].UpperVal);  /* copy TLS sequence number (nonce_explicit) */
    IPBASE_PUT_UINT32(&TempIv[0], 8, Tls_TxSeqNum[TConnId].LowerVal);

    /* fill temp array with the 'associated data' */
    /* additional_data = seq_num (8 byte) + TLSCompressed.type (1 byte, content type) + TLSCompressed.version (2 byte, ProtcolVersion)
    + TLSCompressed.length (2 byte, length of the compressed data) */
    /*
      Byte  0 ..  7: sequencence number
      Byte        8: content type
      Byte  9 .. 10: TLS protocol version
      Byte 11 .. 12: length of the data that shall be authenticated
    */
    IPBASE_PUT_UINT32(&TempAuthData[0], 0, Tls_TxSeqNum[TConnId].UpperVal);
    IPBASE_PUT_UINT32(&TempAuthData[0], 4, Tls_TxSeqNum[TConnId].LowerVal);
    TempAuthData[ 8] = Type;
    TempAuthData[ 9] = TLS_MAJOR_VER;
    TempAuthData[10] = TLS_MINOR_VER_TLS12;  /* AEAD cuipher suites are only supported for TLS1.2 */
    AuthDataLenValue = DataLen;
    if(Type != TLS_REC_CONTENT_HS)
    {
      AuthDataLenValue += TLS_AES_GCM_AD_EXTRA_LEN;
    }
    IPBASE_PUT_UINT16(&TempAuthData[0], 11, AuthDataLenValue);  /* set length value */

    OutputPtr = (uint8*)&(Tls_VCfgGetTxCipherTextStarts()[TConnId][0]);
    OutputLen = 0;  /* length value will be updated by the encryption function */
    (void)esl_initWorkSpaceHeader(&TempWsPtr->header, ESL_MAXSIZEOF_WS_GCM, NULL_PTR);  /* PRQA S 3393,3395 */ /* MD_TLS_12.1_3393_3395 */
    if( (ESL_ERC_NO_ERROR == esl_initEncryptGCM(TempWsPtr, AesGcmKeyPtr, TLS_AES128_KEY_LEN, AesGcmIvPtr, TLS_AES_GCM_TOTAL_IV_LENGTH)) &&  /* AES write key + Nonce */  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
        (ESL_ERC_NO_ERROR == esl_updateAuthDataEncryptGCM(TempWsPtr, &TempAuthData[0], TLS_AES_GCM_AUTH_DATA_LEN)) &&  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
        (ESL_ERC_NO_ERROR == esl_updatePlaintextEncryptGCM(TempWsPtr, DataPtr, DataLen, &OutputPtr[OutputLen], &OutputLen)) )
    {
      if(Type != TLS_REC_CONTENT_HS)
      {
        CONST(uint8, TLS_CONST) InputExtraLenField[TLS_AES_GCM_AD_EXTRA_LEN] = {13, 10};  /* AD extra length field (constant values) */
        ErrorCode = esl_updatePlaintextEncryptGCM(TempWsPtr, &InputExtraLenField[0], TLS_AES_GCM_AD_EXTRA_LEN, &OutputPtr[OutputLen], &OutputLen);
      }
      else
      {
        ErrorCode = ESL_ERC_NO_ERROR;
      }

      if( (ErrorCode == ESL_ERC_NO_ERROR) &&  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
          (ESL_ERC_NO_ERROR == esl_finalizeEncryptGCM(TempWsPtr, &OutputPtr[OutputLen], &OutputLen, &TagField[0])) )
      {
        /* encryption was successful */
      }
      else
      {
        /* error during encryption */
        return E_NOT_OK;
      }
    }
    else
    {
      /* error during encryption */
      return E_NOT_OK;
    }

    PayloadLen = (uint16)(TLS_AES_GCM_RECORD_IV_LENGTH + OutputLen + TLS_AES_BLOCK_LEN);

    /* only adapt length field, re-use other values (Buf[0]..Buf[2]) from encoding */
    IPBASE_PUT_UINT16(&RlHdrBufPtr[0], 3, PayloadLen);

    WriteOffset = 0;

    /* copy record layer header */
    (void)IpBase_CopyString2PbufAt(&RlHdrBufPtr[0], TLS_RL_HDR_LEN, PbufPtr, WriteOffset);
    WriteOffset += TLS_RL_HDR_LEN;

    /* copy nonce_explicit (a sequence) */
    (void)IpBase_CopyString2PbufAt( &TempIv[4], TLS_AES_GCM_RECORD_IV_LENGTH, PbufPtr, WriteOffset);
    WriteOffset += TLS_AES_GCM_RECORD_IV_LENGTH;

    /* copy upper TLS layer content (header, data, ...) */
    /* copy encrypted payload */
    (void)IpBase_CopyString2PbufAt( &((uint8*)(Tls_VCfgGetTxCipherTextStarts()[TConnId]))[0], (uint16)OutputLen, PbufPtr, WriteOffset);
    WriteOffset += OutputLen;

    /* copy TagField */
    (void)IpBase_CopyString2PbufAt(&TagField[0], TLS_AES_BLOCK_LEN, PbufPtr, WriteOffset);
    /* WriteOffset += TLS_AES_BLOCK_LEN;  -> this update is not necessary */

    /* send */
    if(E_OK == TcpIp_TransmitToInt(socketId, PbufPtr, NULL_PTR))
    {
      TLS_LOG_RL_1("Tls_VRl_Transmit: sent with TLS_ECHD(E)_ECDSA_WITH_AES_128_GCM_SHAX, total RL len was %d", (WriteOffset));

      /* inc sequence number */
      Tls_VSeqNumInc(&Tls_TxSeqNum[TConnId]);

      return E_OK;
    }
    else
    {
      /* transmit failed, so free the buffer */
      PbufPtr->len    = 0;
      PbufPtr->totLen = 0;
      (void)TcpIp_TransmitToInt(socketId, PbufPtr, NULL_PTR);
      return E_NOT_OK;
    }
  }
  else
  {
    /* no buffer could be provided by TCP */
    return E_NOT_OK;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  Tls_VRl_RxHdr
 **********************************************************************************************************************/
/*! \brief      Receive Indication Callback
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  PbufPtr        pointer to the received data
 *  \param[in]  StartOffset    header start offset in pbuf
 *  \return                    consumed length, (part of) RL header length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint8, TLS_CODE) Tls_VRl_RxHdr(
    uint8 TConnId,
    P2CONST(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset )
{
  uint32 LocRemLen;
  uint16 LocReadPos;
  uint8  RecLen;
  uint8  Byte;

  LocRemLen  = PbufPtr->totLen - StartOffset;
  LocReadPos = (uint16)StartOffset;

  if( LocRemLen >= (TLS_RL_HDR_LEN - Tls_Rl_HdrReadPos[TConnId]) )
  {
    /* RL header is completed in this message */
    RecLen = (TLS_RL_HDR_LEN - Tls_Rl_HdrReadPos[TConnId]);
  }
  else
  {
    RecLen = (uint8)(LocRemLen);  /* header length max is 5 byte */
  }

  while(RecLen != 0)
  {
    switch(Tls_Rl_HdrReadPos[TConnId])
    {
    case(0):
      {
        /* read content type */
        (void)IpBase_ReadByteInPbuf(PbufPtr, LocReadPos, &Tls_RxProtocol[TConnId]);
        break;
      }
    case(1):
      {
        /* check protocol major version */
        (void)IpBase_ReadByteInPbuf(PbufPtr, LocReadPos, &Byte);
        if(TLS_MAJOR_VER != Byte)
        {
          /* error, version not supported by this implementation */
          return 0;
        }
        break;
      }
    case(2):
      {
        /* check protocol minor version */
        (void)IpBase_ReadByteInPbuf(PbufPtr, LocReadPos, &Byte);
        switch(Byte)
        {
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
        case TLS_MINOR_VER_TLS12:
#endif
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
        case TLS_MINOR_VER_TLS10:
#endif
          {
            /* version is ok */
            break;
          }
        default:
          {
            /* error, version not supported by this implementation */
            return 0;
          }
        }

        break;
      }
    case(3):
      {
        /* read length field */
        (void)IpBase_ReadByteInPbuf(PbufPtr, LocReadPos, &Byte);
        Tls_Rl_TotalPayloadLen[TConnId] = (uint16)((uint16)Byte << 8);
        break;
      }
    case(4):
      {
        /* read length field */
        (void)IpBase_ReadByteInPbuf(PbufPtr, LocReadPos, &Byte);
        Tls_Rl_TotalPayloadLen[TConnId] += Byte;  /* UL payload + MAC length + padding */

        /* prepare some variables and go to next element */
        Tls_Rl_RemPayloadLen[TConnId]     = Tls_Rl_TotalPayloadLen[TConnId];
        Tls_Rl_CurrentPart[TConnId]       = TLS_RL_IN_DATA;
        Tls_Ul_CurrentElement[TConnId]    = TLS_HS_S_IN_TYPE;

        TLS_LOG_RL_2("Tls_VRl_RxHdr: Header completed, content type is %d, payload len is %d", Tls_RxProtocol[TConnId], Tls_Rl_TotalPayloadLen[TConnId]);

        if((Tls_VCfgGetRxPlainTextSizes()[TConnId] - Tls_RxPlainTextFillLevel[TConnId]) < Tls_Rl_TotalPayloadLen[TConnId])
        {
          /* error, message too long (for this implementation), or the application did not release enough of the rx buffer */
          TLS_CallDetReportError(TLS_API_ID_V_RL_RX_HDR, TLS_E_BUF_SIZE)
          Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
          return 0;
        }

        break;
      }
    default:
      {
        /* do nothing */
        break;
      }
    }
    RecLen--;
    LocReadPos++;
    Tls_Rl_HdrReadPos[TConnId]++;
  }

  if(TLS_RL_HDR_LEN == Tls_Rl_HdrReadPos[TConnId])
  {
    /* RL header is read completely, reset header read pos */
    Tls_Rl_HdrReadPos[TConnId] = 0;

    /* release the 5 bytes in the TLS and TCP buffer */
    Tls_VReleaseBufSegment(TConnId, &Tls_RxBufSegMgt[TConnId], TLS_RL_HDR_LEN);
  }

  /* return consumed length: */
  return (uint8)(LocReadPos - (uint16)StartOffset);

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  Tls_VRl_RxPayload
 **********************************************************************************************************************/
/*! \brief      Receive payload part of record layer
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  PbufPtr        pointer to the received data
 *  \param[in]  StartOffset    data start offset in pbuf
 *  \param[in]  RxLen          data length that shall be received
 *  \return                    consumed length, (part of) RL header length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VRl_RxPayload(
    uint8 TConnId,
    P2VAR(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset,
    uint16 RxLen )
{
  Std_ReturnType retVal    = E_NOT_OK;
  uint16         returnLen;
  uint16 initialRlRemPayloadLen = Tls_Rl_RemPayloadLen[TConnId];

  /* decrypt data */
  /* Differ depending on active cipher algorithm */
  switch(Tls_SecParamRxActivePtr[TConnId]->bulk_cipher_algorithm)
  {
  case TLS_BULK_CIPHER_ALGORITHM_NULL:
    {
      retVal = Tls_VRl_RxPayloadWithNull(TConnId, PbufPtr, StartOffset, RxLen);
      break;
    }
  case TLS_BULK_CIPHER_ALGORITHM_AES128_CBC:
    {
      retVal = Tls_VRl_RxPayloadWithAesCbc(TConnId, PbufPtr, StartOffset, RxLen);
      break;
    }
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
#if (TLS_SUPPORT_AES_GCM == STD_ON)
  /* AEAD-Ciphers are only supported by min TLS1.2 */
  /*  */
  case TLS_BULK_CIPHER_ALGORITHM_AES128_GCM:
    {
      retVal = Tls_VRl_RxPayloadWithAesGcm(TConnId, PbufPtr, StartOffset, RxLen);
      break;
    }
#endif  /* (TLS_SUPPORT_AES_GCM == STD_ON) */
#endif  /* (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON) */
  default:
    {
      /* Invalid configuration, not supported TLS suite is chosen
         -> this error condition is already checked during handshake, so nothing has to be done here */
#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
        TLS_CallDetReportError(TLS_API_ID_V_RL_RX_PAYLOAD, TLS_E_UNEXPECTED_CFG_ERROR)
#endif
    }
  }

  /* ------------------------------------------------------------------ */
  /* analyze content depending on content type (TLS UL protocol) */

  if(retVal == E_OK)
  {
    uint16 receivedLen;
    if(Tls_Rl_RemPayloadLen[TConnId] != 0)
    {
      /* not the whole RL messages was received, there will be another part of the message in the next TCP segment */
      receivedLen = RxLen;  /* the whole length can be released */
    }
    else
    {
      if(RxLen == Tls_Rl_TotalPayloadLen[TConnId])
      {
        /* the TCP receive buffer content exactly filled the current RL message, there is no further data left in the buffer */
        receivedLen = RxLen;
      }
      else
      {
        /* there are further RL messages (or part of it) in the receive buffer, so only release the part of the buffer that was currently consumed */
        receivedLen = initialRlRemPayloadLen;
      }
    }
    /* release the rx buffer in TCP */
    Tls_VReleaseBufSegment(TConnId, &Tls_RxBufSegMgt[TConnId], receivedLen);

    if(0 == Tls_Rl_RemPayloadLen[TConnId])
    {
      retVal = Tls_VRl_ForwardRxData(TConnId);
      Tls_Rl_TotalPayloadLen[TConnId] = 0;  /* reset payload length */
    }

  } /* end of handling of decrypted payload */

  /* return consumed length */
  if(retVal == E_OK)
  {
    returnLen = RxLen;
  }
  else
  {
    returnLen = 0;
  }

  return returnLen;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VRl_RxPayloadWithNull
 **********************************************************************************************************************/
/*! \brief      Receive payload part of record layer, using Null decryption
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  PbufPtr        pointer to the received data
 *  \param[in]  StartOffset    data start offset in pbuf
 *  \param[in]  RxLen          data length that shall be received
 *  \return     E_OK           no failure during decryption <br>
 *              E_NOT_OK       failure during decryption
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_RxPayloadWithNull(
    uint8 TConnId,
    P2CONST(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset,
    uint16 RxLen )
{
  /* cipher suite TLS_xx_WITH_NULL_xx 
     - TLS_NULL_WITH_NULL_NULL
     - TLS_RSA_WITH_NULL_SHA
     - TLS_RSA_WITH_NULL_SHA256
     - TLS_ECDH_ECDSA_WITH_NULL_SHA */

  Std_ReturnType retVal = E_NOT_OK;

  if(TLS_NULL_WITH_NULL_NULL == Tls_SecParamRxActivePtr[TConnId]->cipher_suite)
  {
    /* TLS_NULL_WITH_NULL_NULL */
    /* initial cipher suite, without key exchange, encryption and MAC */

    /* copy data to linear output buffer */
    if(E_OK == IpBase_CopyPbuf2String( &((Tls_GetRxPlainTextStartPtr(TConnId))[Tls_RxPlainTextFillLevel[TConnId]]),
                                             PbufPtr,
                                             RxLen,
                                             StartOffset ))
    {
      Tls_RxPlainTextFillLevel[TConnId] += RxLen;
      Tls_Rl_RemPayloadLen[TConnId]     -= RxLen;

      if(0 == Tls_Rl_RemPayloadLen[TConnId])
      {
        /* RL payload received completely */
        Tls_Rl_DecryptedDataLen[TConnId] = Tls_Rl_TotalPayloadLen[TConnId];
      }

      retVal = E_OK;
    }
    else
    {
      /* error, could not copy pbuf to string */
    }
  }
  else
  {
    /* cipher suites xx_WITH_NULL_xx
       - TLS_RSA_WITH_NULL_SHA
       - TLS_RSA_WITH_NULL_SHA256
       - TLS_ECDH_ECDSA_WITH_NULL_SHA */
    uint8   MacLen;

    TLS_LOG_RX_IND_2("Tls_VRl_RxPayloadWithNull not NULL_NULL: receive %d byte data, Pbuf total length is %d", RxLen, PbufPtr->totLen);

    if( (TLS_RSA_WITH_NULL_SHA == Tls_SecParamRxActivePtr[TConnId]->cipher_suite) ||
        (TLS_ECDH_ECDSA_WITH_NULL_SHA == Tls_SecParamRxActivePtr[TConnId]->cipher_suite) )
    {
      MacLen = TLS_HASH_SHA1_LEN;
    }
    else
    {
      MacLen = TLS_HASH_SHA256_LEN;
    }

    /* copy data to linear output buffer */
    if(E_OK == IpBase_CopyPbuf2String( Tls_GetRxPlainWritePtr(TConnId),
                                             PbufPtr,
                                             RxLen,
                                             StartOffset ))
    {
      Tls_RxPlainTextFillLevel[TConnId] += RxLen;
      Tls_Rl_RemPayloadLen[TConnId]     -= RxLen;

      if(0 == Tls_Rl_RemPayloadLen[TConnId])
      {
        /* RL payload received completely */
        Tls_Rl_DecryptedDataLen[TConnId]     = Tls_Rl_TotalPayloadLen[TConnId] - MacLen;  /* remove MAC from the decrypted payload */
        Tls_RxPlainTextFillLevel[TConnId]   -= MacLen;  /* remove MAC from the decrypted payload */

        /* check MAC */
        if(E_OK != Tls_VRl_CheckRxMac(TConnId))
        {
          /* error while checking the MAC */
          Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_BAD_RECORD_MAC);
        }
        else
        {
          retVal = E_OK;
        }
      }
      else
      {
        retVal = E_OK;
      }
    }
    else
    {
      /* error, could not copy pbuf to string */
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 *  Tls_VRl_RxPayloadWithAesCbc
 **********************************************************************************************************************/
/*! \brief      Receive payload part of record layer, using AES_CBC for decryption
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  PbufPtr        pointer to the received data
 *  \param[in]  StartOffset    data start offset in pbuf
 *  \param[in]  RxLen          data length that shall be received
 *  \return     E_OK           no failure during decryption <br>
 *              E_NOT_OK       failure during decryption
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_RxPayloadWithAesCbc(
    uint8 TConnId,
    P2VAR(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset,
    uint16 RxLen )
{
  /* cipher suites with encryption 'AES128_CBC' (cipher_suites TLS_xx_WITH_AES_128_CBC_xx)
     - case TLS_RSA_WITH_AES_128_CBC_SHA
     - case TLS_RSA_WITH_AES_128_CBC_SHA256
     - case TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA
     - case TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
     - case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
     - case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 */

  /* encryption = AES 128 CBC */

  Std_ReturnType retVal = E_OK;

  /* ---------------------------------------------------------------------- */
  /* copy pbuf to linear buffer */
  boolean IsLastPart;
  uint16  DecryptedLen;
  uint16  totalDecryptedLen = 0;
  Tls_PbufType *LocPbufPtr;
  uint16  RemStartOffset;
  uint16  RemRxLen;
  uint8   KeyPartOffsIv;
  uint8   MacLen;

  if(Tls_Rl_RemPayloadLen[TConnId] != 0)
  {
    totalDecryptedLen = (uint16)(((uint16)(Tls_Rl_TotalPayloadLen[TConnId] - Tls_Rl_RemPayloadLen[TConnId])) & 0xFFF0u);
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    if((TLS_PROT_VER_TLS12 == Tls_ProtVer[TConnId]) && (totalDecryptedLen >= TLS_AES_BLOCK_LEN))
    {
      totalDecryptedLen -= TLS_AES_BLOCK_LEN;  /* reduce by length of the IV-Vector */
    }
#endif
  }

  switch(Tls_SecParamRxActivePtr[TConnId]->cipher_suite)
  {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
  case(TLS_RSA_WITH_AES_128_CBC_SHA):
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
  case(TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA):
  case(TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA):
#endif
    {
      KeyPartOffsIv = TLS_KEY_PART_OFFSET_AES128_SHA1_IV;
      MacLen = TLS_HASH_SHA1_LEN;
      break;
    }
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
  case(TLS_RSA_WITH_AES_128_CBC_SHA256):
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
  case(TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256):
  case(TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256):
#endif
    {
      KeyPartOffsIv = TLS_KEY_PART_OFFSET_AES128_SHA256_IV;
      MacLen = TLS_HASH_SHA256_LEN;
      break;
    }
  default:
    {
      /* error, invalid config, a not supported cipher suite was chosen */
#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
      TLS_CallDetReportError(TLS_API_ID_V_RL_RX_PAYLOAD, TLS_E_INV_PARAM)
#endif
      KeyPartOffsIv = 0;
      MacLen        = 0;
      retVal        = E_NOT_OK;
    }
  }

  if(retVal == E_NOT_OK)
  {
    /* an error occurred in the previous code section, so skip the next code block */
  }
  else
  {
    /* go to start inside the Pbuf */
    LocPbufPtr     = PbufPtr;
    RemStartOffset = StartOffset;
    while(RemStartOffset >= LocPbufPtr->len)  /*lint !e661 */ /* pbuf element is only accessed if available */
    {
      RemStartOffset -= LocPbufPtr->len;
      LocPbufPtr = &LocPbufPtr[1];
    }

    /* parse those parts of the PBuf that contain the received payload */
    RemRxLen = RxLen;
    while((RemRxLen != 0) && (retVal == E_OK))
    {
      uint8 *LocCiphTxtPtr = &(LocPbufPtr->payload[RemStartOffset]);
      uint16 LocCiphTxtLen = LocPbufPtr->len - RemStartOffset;
      if(RemRxLen < LocCiphTxtLen)
      {
        LocCiphTxtLen = RemRxLen;
      }

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
      if( (TLS_PROT_VER_TLS12 == Tls_ProtVer[TConnId]) && (TLS_AES_BLOCK_LEN != Tls_RlRxIvFillLevel[TConnId]) )
      {
        /* Copy received IV:
        For Tls 1.2 the first bytes contain the IV for AES128 (length is TLS_AES_BLOCK_LEN = 16 byte), and they are not
        encrypted. Copy (partially) received IV to (&(((uint8*)(Tls_KeyMatBlockRxActivePtr[TConnId]))[KeyPartOffsIv])) */
        uint8 *RxIvPtr = &(((uint8*)(Tls_KeyMatBlockRxActivePtr[TConnId]))[KeyPartOffsIv + Tls_RlRxIvFillLevel[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
        uint8 CopyLen;
        /* read iv and inc read position */
        if(LocCiphTxtLen >= (TLS_AES_BLOCK_LEN - Tls_RlRxIvFillLevel[TConnId]))
        {
          /* copy (last part of) the IV: */
          CopyLen = (TLS_AES_BLOCK_LEN - Tls_RlRxIvFillLevel[TConnId]);
          (void)IpBase_CopyPbuf2String( RxIvPtr, LocPbufPtr, CopyLen, RemStartOffset);
          Tls_RlRxIvFillLevel[TConnId] += CopyLen;
          RemRxLen               -= CopyLen;
          LocCiphTxtLen          -= CopyLen;
          LocCiphTxtPtr = &LocCiphTxtPtr[CopyLen];
        }
        else
        {
          /* copy first or middle part of the IV: */
          CopyLen = (uint8)LocCiphTxtLen;  /* length is always smaller than 255 */
          (void)IpBase_CopyPbuf2String( RxIvPtr, LocPbufPtr, CopyLen, RemStartOffset);
          Tls_RlRxIvFillLevel[TConnId] += CopyLen;
          RemRxLen               -= CopyLen;
          LocCiphTxtLen           = 0;
        }

        Tls_Rl_RemPayloadLen[TConnId] -= CopyLen;
      }
#endif

      /* decrypt the payload (or part of it) */
      if(LocCiphTxtLen == Tls_Rl_RemPayloadLen[TConnId])
      {
        IsLastPart = TRUE;
      }
      else
      {
        IsLastPart = FALSE;
      }
      DecryptedLen = Tls_VCfgGetRxPlainTextSizes()[TConnId] - Tls_RxPlainTextFillLevel[TConnId];  /* set maximum free buffer size */
      if(0 != LocCiphTxtLen)
      {
        if(TLS_E_OK == Tls_VAesDecryptData( TConnId,
          (&(((uint8*)(Tls_KeyMatBlockRxActivePtr[TConnId]))[KeyPartOffsIv])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
          LocCiphTxtPtr,
          LocCiphTxtLen,
          &((Tls_GetRxPlainTextStartPtr(TConnId))[Tls_RxPlainTextFillLevel[TConnId]]),
          &DecryptedLen,
          IsLastPart ))
        {
          Tls_RxPlainTextFillLevel[TConnId] += DecryptedLen;
          Tls_Rl_RemPayloadLen[TConnId]     -= LocCiphTxtLen;

#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
          if(TLS_PROT_VER_TLS10 == Tls_ProtVer[TConnId])
#endif
          {
            if(Tls_Rl_RemPayloadLen[TConnId] < TLS_AES_BLOCK_LEN)
            {
              /* copy last encrypted block */
              /* length to copy = Tls_Rl_TotalPayloadLen - Tls_Rl_RemPayloadLen[TConnId] */
              uint16 CopyLen;
              uint16 CopyLen2;
              uint16 CopyOffset;
              uint16 PBufOffset;
              uint8 *NextIvTgtPtr;

              CopyLen2 = TLS_AES_BLOCK_LEN - Tls_Rl_RemPayloadLen[TConnId];
              if(CopyLen2 > RxLen)
              {
                /* very short TCP segment received, only inner part of one AES block */
                CopyLen = RxLen;
                CopyOffset = TLS_AES_BLOCK_LEN - (Tls_Rl_RemPayloadLen[TConnId] + RxLen);
                PBufOffset = 0;
              }
              else
              {
                CopyLen = CopyLen2;
                CopyOffset = 0;
                PBufOffset = RxLen - (TLS_AES_BLOCK_LEN - Tls_Rl_RemPayloadLen[TConnId]);
              }
              NextIvTgtPtr = &Tls_TxNextIv[(TLS_AES_BLOCK_LEN * TConnId) + CopyOffset];

              /* copy */
              (void)IpBase_CopyPbuf2String(NextIvTgtPtr, PbufPtr, CopyLen, (StartOffset + PBufOffset));
            }
          }
#endif
          if(TRUE == IsLastPart)
          {
            /* if payload is received completly, check the MAC and prepare for receiving the next message */
            Tls_Rl_DecryptedDataLen[TConnId]   = (totalDecryptedLen + DecryptedLen) - (MacLen + 1);  /* remove MAC and padding from the decrypted payload */
            Tls_RxPlainTextFillLevel[TConnId] -= (MacLen + 1);  /* remove MAC and padding length from the decrypted payload */

            /* check MAC */
            if(E_OK != Tls_VRl_CheckRxMac(TConnId))
            {
              /* error while checking the MAC */
              Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_BAD_RECORD_MAC);
            }

            switch(Tls_ProtVer[TConnId])
            {
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
            case TLS_PROT_VER_TLS12:
              {
                /* TLS 1.2 */
                /* reset received iv length */
                Tls_RlRxIvFillLevel[TConnId] = 0;
                break;
              }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
            case TLS_PROT_VER_TLS10:
              {
                /* TLS 1.0 */
                /* update IV */
                uint8 *IvStoragePtr;
                IvStoragePtr = (&(((uint8*)(Tls_KeyMatBlockRxActivePtr[TConnId]))[KeyPartOffsIv]));  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
                IpBase_Copy((IpBase_CopyDataType *)(IvStoragePtr), (const IpBase_CopyDataType *)(&Tls_TxNextIv[TLS_AES_BLOCK_LEN * TConnId]), TLS_AES_BLOCK_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
                break;
              }
#endif
            default:
              {
                /* one of the above elements will always be true -> do nothing here */
              }
            }

          }
          else
          {
            totalDecryptedLen += DecryptedLen;
            retVal = E_OK;
          }
        }
        else
        {
          /* error while decrypting the data (AES) */
          Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECRYPT_ERROR);

          retVal = E_NOT_OK;  /* break the while loop */
        }
      }

      if(retVal == E_OK)
      {
        RemRxLen -= LocCiphTxtLen;
        RemStartOffset = 0;           /* reset offset in PBuf */
        LocPbufPtr = &LocPbufPtr[1];  /* go to next PBuf element */
      }
    }
  }

  return retVal;
}  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
#if (TLS_SUPPORT_AES_GCM == STD_ON)
/***********************************************************************************************************************
 *  Tls_VRl_RxPayloadWithAesGcm
 **********************************************************************************************************************/
/*! \brief      Receive payload part of record layer, using AES_GCM for decryption
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  PbufPtr        pointer to the received data
 *  \param[in]  StartOffset    data start offset in pbuf
 *  \param[in]  RxLen          data length that shall be received
 *  \return     E_OK           no failure during decryption <br>
 *              E_NOT_OK       failure during decryption
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_RxPayloadWithAesGcm(
    uint8 TConnId,
    P2VAR(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) PbufPtr,
    uint16 StartOffset,
    uint16 RxLen )
{
  /* cipher suites with encryption 'AES128_GCM' (cipher_suites TLS_xx_WITH_AES_128_GCM_xx)
     - TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
     - TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 */

  Std_ReturnType retVal = E_OK;

  Tls_ReturnType DecryptRetVal;

  /* ---------------------------------------------------------------------- */
  /* copy pbuf to linear buffer */
  boolean IsLastPart;
  uint16  DecryptedLen = 0;
  uint16  NewDecryptedLen;  /* variable shall be used when calling the AES decrypt function. When call the function it shall contain
                            the max free buffer length, when returning from the function it shall contain the new decrypted length. */
  Tls_PbufType *LocPbufPtr;
  uint16  RemStartOffset;
  uint16  RemRxLen;

  TLS_LOG_RL("Tls_VRl_RxPayload: receive with TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256 or TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256");

  /* go to start inside the Pbuf */
  LocPbufPtr     = PbufPtr;
  RemStartOffset = StartOffset;
  while(RemStartOffset >= LocPbufPtr->len)  /*lint !e661 */ /* pbuf element is only accessed if available */
  {
    RemStartOffset -= LocPbufPtr->len;
    LocPbufPtr = &LocPbufPtr[1];
  }

  /* parse those parts of the PBuf that contain the received payload */
  RemRxLen = RxLen;
  while((RemRxLen != 0) && (retVal == E_OK))
  {
    uint8 *LocCiphTxtPtr = &(LocPbufPtr->payload[RemStartOffset]);
    uint16 LocCiphTxtLen = LocPbufPtr->len - RemStartOffset;
    if(RemRxLen < LocCiphTxtLen)
    {
      LocCiphTxtLen = RemRxLen;
    }

    if(TLS_AES_GCM_RECORD_IV_LENGTH != Tls_RlRxIvFillLevel[TConnId])
    {
      /* Copy received IV:
      For Tls 1.2 the first bytes contain the explicit nonce for AES128GCM (length is TLS_AES_GCM_RECORD_IV_LENGTH = 8 byte), and they are not
      encrypted. Copy (partially) received nonce to &Tls_RxNonceExplicit[TConnId * TLS_AES_GCM_RECORD_IV_LENGTH] */
      uint8 *RxIvPtr = &Tls_RxNonceExplicit[(TConnId * TLS_AES_GCM_RECORD_IV_LENGTH) + Tls_RlRxIvFillLevel[TConnId]];
      uint8 CopyLen;
      /* read iv (explicit nonce) and inc read position */
      if(LocCiphTxtLen >= (TLS_AES_GCM_RECORD_IV_LENGTH - Tls_RlRxIvFillLevel[TConnId]))
      {
        /* copy (last part of) the explicit nonce: */
        CopyLen = (TLS_AES_GCM_RECORD_IV_LENGTH - Tls_RlRxIvFillLevel[TConnId]);
        (void)IpBase_CopyPbuf2String( RxIvPtr, LocPbufPtr, CopyLen, RemStartOffset);
        Tls_RlRxIvFillLevel[TConnId] += CopyLen;
        RemRxLen               -= CopyLen;
        LocCiphTxtLen          -= CopyLen;
        LocCiphTxtPtr = &LocCiphTxtPtr[CopyLen];
      }
      else
      {
        /* copy first or middle part of the explicit nonce: */
        CopyLen = (uint8)LocCiphTxtLen;  /* length is always smaller than 255 */
        (void)IpBase_CopyPbuf2String( RxIvPtr, LocPbufPtr, CopyLen, RemStartOffset);
        Tls_RlRxIvFillLevel[TConnId] += CopyLen;
        RemRxLen               -= CopyLen;
        LocCiphTxtLen           = 0;
      }

      Tls_Rl_RemPayloadLen[TConnId] -= CopyLen;
      RemStartOffset += CopyLen;  /* total is TLS_AES_GCM_RECORD_IV_LENGTH */
    }

    /* decrypt the payload (or part of it) */
    if(LocCiphTxtLen == Tls_Rl_RemPayloadLen[TConnId])
    {
      IsLastPart = TRUE;
    }
    else
    {
      IsLastPart = FALSE;
    }

    {
      /* Copy received TagField:
      For Tls 1.2 the last bytes contain the TagField for AES128GCM (length is TLS_AES_BLOCK_LEN = 16 byte), and
      they are not encrypted. Copy (partially) received TagField to Tls_GcmRxTagField[] */
      uint16 TagOffset;  /* start offset (in the PBuf) of the TagField */
      uint8 CopyLen;     /* length of the TagField part in this rx element */

      if(Tls_Rl_RemPayloadLen[TConnId] > TLS_AES_BLOCK_LEN)
      {
        /* the current rx element starts somewhere in the encrypted data block */
        if(LocCiphTxtLen < (Tls_Rl_RemPayloadLen[TConnId] - TLS_AES_BLOCK_LEN))
        {
          /* the current rx element only contains encrypted data (no part of the TagField !!)
          -> nothing to copy here */
          CopyLen = 0;
          TagOffset = 0;
        }
        else
        {
          /* the current rx element contains encrypted data and (a part of) the TagField */
          /* copy the TAG part of the message */
          CopyLen = (uint8)(LocCiphTxtLen - (Tls_Rl_RemPayloadLen[TConnId] - TLS_AES_BLOCK_LEN));
          TagOffset = (uint16)(RemStartOffset + (uint16)(Tls_Rl_RemPayloadLen[TConnId] - TLS_AES_BLOCK_LEN));
        }
      }
      else
      {
        /* the current rx element starts somewhere in the TagField */
        CopyLen = (uint8)Tls_Rl_RemPayloadLen[TConnId];
        TagOffset = 0;
      }

      if(CopyLen != 0)
      {
        uint8 *TagStoragePtr = &Tls_GcmRxTagField[(TConnId * TLS_AES_BLOCK_LEN) + Tls_GcmRxTagFieldFillLevel[TConnId]];

        (void)IpBase_CopyPbuf2String( TagStoragePtr, LocPbufPtr, CopyLen, TagOffset);
        Tls_GcmRxTagFieldFillLevel[TConnId] += CopyLen;
      }
    }

    NewDecryptedLen = Tls_VCfgGetRxPlainTextSizes()[TConnId] - Tls_RxPlainTextFillLevel[TConnId];  /* set length to currently free buffer length */
    if(0 != LocCiphTxtLen)
    {
      DecryptRetVal = Tls_VAesGcmDecryptData( TConnId,
                                              (uint8*)&Tls_WorkSpaceAes[TConnId],  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
                                              LocCiphTxtPtr, LocCiphTxtLen,
                                              &((Tls_GetRxPlainTextStartPtr(TConnId))[Tls_RxPlainTextFillLevel[TConnId]]),
                                              &NewDecryptedLen,
                                              IsLastPart );

      DecryptedLen += NewDecryptedLen;

      if( (TLS_E_OK == DecryptRetVal) && (TRUE == IsLastPart) && (Tls_RxProtocol[TConnId] != TLS_REC_CONTENT_HS) )
      {
        /* check the AD extra length, and remove it from plain text */
        CONST(uint8, TLS_CONST) InputExtraLenField[TLS_AES_GCM_AD_EXTRA_LEN] = { 13, 10 };  /* AD extra length field (constant values) */

        if(IPBASE_CMP_EQUAL !=IpBase_StrCmpLen(&Tls_GetRxPlainTextStartPtr(TConnId)[DecryptedLen - TLS_AES_GCM_AD_EXTRA_LEN], &InputExtraLenField[0], TLS_AES_GCM_AD_EXTRA_LEN))
        {
          DecryptRetVal = TLS_E_NOT_OK;
        }
        DecryptedLen -= TLS_AES_GCM_AD_EXTRA_LEN;
      }

      if(TLS_E_OK == DecryptRetVal)
      {
        Tls_RxPlainTextFillLevel[TConnId] = DecryptedLen;
        Tls_Rl_RemPayloadLen[TConnId]     -= LocCiphTxtLen;

        if(TRUE == IsLastPart)
        {
          /* if payload is received completly, prepare for receiving the next message */
          Tls_Rl_DecryptedDataLen[TConnId] = Tls_RxPlainTextFillLevel[TConnId];

          /* reset received iv length */
          Tls_RlRxIvFillLevel[TConnId] = 0;
        }
      }
      else
      {
        /* error while decrypting the data (AES) */
        Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECRYPT_ERROR);

        retVal = E_NOT_OK;  /* leave the function immediatelly */
      }
    }

    if(retVal == E_OK)
    {
      RemRxLen -= LocCiphTxtLen;
      RemStartOffset = 0;           /* reset offset in PBuf */
      LocPbufPtr = &LocPbufPtr[1];  /* go to next PBuf element */
    }
  }

  return retVal;

}  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
*  Tls_VAesGcmDecryptData
**********************************************************************************************************************/
/* \brief         Decrypts data using AES encryption with 128 Bit blocksize in GCM mode
*  \param[in]     TConnId               TLS connection index
*  \param[in]     WsPtr                 Pointer to crypto workspace
*  \param[in]     CipherTextPtr         Cipher text to be decrypted
*  \param[in]     CipherTextLength      Cipher text length
*  \param[in]     PlainTextPtr          Pointer to buffer element where to store the decrypted data
*  \param[in,out] PlainTextLengthPtr    In: Max buffer size, out: plain text length
*  \param[in]     Finished              TRUE if cipher text includes last byte to be encrypted, padding will be removed
*  \return        TLS_E_OK              Decryption finished successfully
*  \return        TLS_E_NOT_OK          Decryption error
*  \return        TLS_E_SMALL_BUFFER    Target buffer to small to store the plain text, call again with more buffer
*  \context       task level
**********************************************************************************************************************/
STATIC FUNC(Tls_ReturnType, TLS_CODE) Tls_VAesGcmDecryptData(
    uint8 TConnId,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_DATA) WsPtr,
    P2CONST(uint8,  AUTOMATIC, TLS_APPL_DATA) CipherTextPtr,
    uint16 CipherTextLength,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_DATA) PlainTextPtr,
    P2VAR(uint16,  AUTOMATIC, TLS_APPL_DATA) PlainTextLengthPtr,
    boolean Finished )
{
  Tls_ReturnType      result       = TLS_E_OK;
  eslt_ErrorCode      eslResult;
  uint32              locOutputLen = 0;  /* tmpLen used to determin the total plain text data size in case multiple calls to the es-lib are executed */
  eslt_WorkSpaceGCM  *aesGcmWsPtr  = (eslt_WorkSpaceGCM*)WsPtr;  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_param */

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_AES_GCM_DECRYPT_DATA, TLS_E_INV_PARAM, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(WsPtr)), TLS_API_ID_AES_GCM_DECRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(PlainTextPtr)), TLS_API_ID_AES_GCM_DECRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(CipherTextPtr)), TLS_API_ID_AES_GCM_DECRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(PlainTextLengthPtr)), TLS_API_ID_AES_GCM_DECRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
#endif

  /* no decryption in process for this connection, init workspace with key and IV */
  if(TLS_ENC_STATE_IDLE == Tls_AESDecryptionState[TConnId])
  {
    uint8 TempAuthData[13];
    uint8 TempIv[12];
    uint16 AuthDataLenValue;

    IPBASE_PUT_UINT32(&TempAuthData[0], 0, Tls_RxSeqNum[TConnId].UpperVal);
    IPBASE_PUT_UINT32(&TempAuthData[0], 4, Tls_RxSeqNum[TConnId].LowerVal);
    TempAuthData[ 8] = Tls_RxProtocol[TConnId];
    TempAuthData[ 9] = TLS_MAJOR_VER;
    TempAuthData[10] = TLS_MINOR_VER_TLS12;  /* AEAD cuipher suites are only supported for TLS1.2 */
    AuthDataLenValue = (uint16)(Tls_Rl_TotalPayloadLen[TConnId] - (TLS_AES_GCM_RECORD_IV_LENGTH + TLS_AES_BLOCK_LEN));
    IPBASE_PUT_UINT16(&TempAuthData[0], 11, AuthDataLenValue);  /* set length value */

    /* merge both parts of the IV */
    IpBase_Copy((IpBase_CopyDataType*)(&TempIv[0]), (const IpBase_CopyDataType*)(Tls_SecParamRxActivePtr[TConnId]->IvPtr), TLS_AES_GCM_FIXED_IV_LENGTH);
    IpBase_Copy((IpBase_CopyDataType*)(&TempIv[TLS_AES_GCM_FIXED_IV_LENGTH]), (const IpBase_CopyDataType*)(&Tls_RxNonceExplicit[TConnId * TLS_AES_GCM_RECORD_IV_LENGTH]), TLS_AES_GCM_RECORD_IV_LENGTH);

    (void)esl_initWorkSpaceHeader(&aesGcmWsPtr->header, ESL_MAXSIZEOF_WS_GCM, NULL_PTR);  /* PRQA S 3393,3395 */ /* MD_TLS_12.1_3393_3395 */
    if( (ESL_ERC_NO_ERROR == esl_initDecryptGCM(aesGcmWsPtr, Tls_SecParamRxActivePtr[TConnId]->KeyPtr, TLS_AES128_KEY_LEN, &TempIv[0], TLS_AES_GCM_TOTAL_IV_LENGTH)) &&  /* AES write key + Nonce */  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
      (ESL_ERC_NO_ERROR == esl_updateAuthDataDecryptGCM(aesGcmWsPtr, &TempAuthData[0], TLS_AES_GCM_AUTH_DATA_LEN)) )
    {
      /* Initialisation finished successfully, update decryption state for this connection */
      Tls_AESDecryptionState[TConnId] = TLS_ENC_STATE_RUNNING;
    }
    else
    {
      /* Error during workspace initialisation */
      TLS_CallDetReportError(TLS_API_ID_V_RL_RX_PAYLOAD, TLS_E_UNEXPECTED_CRYPTO_ERROR)
        result = TLS_E_NOT_OK;
    }
  }

  /* workspace initialized */
  if( (TLS_E_NOT_OK != result) && (TLS_ENC_STATE_RUNNING == Tls_AESDecryptionState[TConnId]) )
  {
    uint16                                     CurrentMaxPlaintextLen;
    uint16                                     DataLen   = CipherTextLength - Tls_GcmRxTagFieldFillLevel[TConnId];
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) OutputPtr = PlainTextPtr;
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA)   DataPtr   = CipherTextPtr;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA)   TagFieldPtr;

    /* store max buffer size, needed for following finalize-call */
    CurrentMaxPlaintextLen = *PlainTextLengthPtr;

    /* decrypt data */
    if(Tls_Rl_RemPayloadLen[TConnId] >= 16)
    {
      if(CipherTextLength < (Tls_Rl_RemPayloadLen[TConnId] - 16))
      {
        DataLen = CipherTextLength;
      }
    }
    else
    {
      DataLen = 0;
    }

    if(CurrentMaxPlaintextLen >= DataLen)
    {
      if(DataLen != 0)
      {
        eslResult = esl_updateCiphertextDecryptGCM(aesGcmWsPtr, DataPtr, DataLen, OutputPtr, &locOutputLen);
        PlainTextPtr = &OutputPtr[locOutputLen];

        /* check error state */
        if(ESL_ERC_NO_ERROR != eslResult)
        {
          /* Decryption error */
          TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_RL_RX_PAYLOAD, TLS_EVENT_AES_MSG_DECRYPTION, TLS_EVENT_STATUS_FAILED)  /* report event */
            result = TLS_E_NOT_OK;
        }
      }
    }
    else
    {
      /* plaintext buffer is too small to decrypt data */
      result = TLS_E_SMALL_BUFFER;
    }

    /* error occured in allready executed decryption steps? */
    if(TLS_E_OK == result)
    {
      /* data decryption finished, last data bytes will be decrypted */
      if(TRUE == Finished)
      {
        /* finalize decryption: */
        /* decrypt last bytes if length is not a multiple of AES block length, and validate the content of the TagField */
        TagFieldPtr = &Tls_GcmRxTagField[(TConnId * TLS_AES_BLOCK_LEN)];
        eslResult = esl_finalizeDecryptGCM(aesGcmWsPtr, PlainTextPtr, &locOutputLen, TagFieldPtr);

        /* check eror state */
        if(ESL_ERC_NO_ERROR != eslResult)
        {
          if(ESL_ERC_OUTPUT_SIZE_TOO_SHORT == eslResult)
          {
            /* more buffer required to decrypt the data */
            result = TLS_E_SMALL_BUFFER;
          }
          else
          {
            /* Decryption error */
            TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_RL_RX_PAYLOAD, TLS_EVENT_COULD_NOT_DECRYPT_MSG, TLS_EVENT_STATUS_FAILED)  /* report event */
              result = TLS_E_NOT_OK;
          }
        }
        else
        {
          Tls_GcmRxTagFieldFillLevel[TConnId] = 0;  /* reset fill level */

          Tls_VSeqNumInc(&Tls_RxSeqNum[TConnId]);  /* inc sequence number */

          /* set plain text length, update decryption state for this connection */
          *PlainTextLengthPtr = (uint16)locOutputLen;
          Tls_AESDecryptionState[TConnId] = TLS_ENC_STATE_IDLE;
        }
      }
      /* another call with more data will follow */
      else
      {
        if(NULL_PTR != PlainTextLengthPtr)  /* PRQA S 3355,3358 */ /* MD_TLS_13.7_3355_3358 */
        {
          /* set plain text length */
          *PlainTextLengthPtr = (uint16)locOutputLen;
        }
      }
    }
  }
  return result;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif  /* (TLS_SUPPORT_AES_GCM == STD_ON) */
#endif  /* (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON) */

/***********************************************************************************************************************
 *  Tls_VRl_ForwardRxData
 **********************************************************************************************************************/
/*! \brief      Forward received data depending on the message type
 *  \param[in]  TConnId          TLS connection id
 *  \return     E_OK             forwarded successfully <br>
 *              E_NOT_OK         error occurred during forwarding the data
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_ForwardRxData(
    uint8 TConnId )
{
  /* handle data
     -> work on decrypted data */
  Std_ReturnType  retVal            = E_NOT_OK;
  uint8          *PlainTextDataPtr;
  uint16          ConsumedLen;
  uint16          MaxRxLen          = Tls_Rl_DecryptedDataLen[TConnId];

  PlainTextDataPtr = &Tls_GetRxPlainTextStartPtr(TConnId)[Tls_RxPlainTextFillLevel[TConnId] - Tls_Rl_DecryptedDataLen[TConnId]];

  switch(Tls_RxProtocol[TConnId])
  {
  case(TLS_REC_CONTENT_AD):  /* application data */
    {
      /* currently receiving application data msg */
      /* Set data length. Length is reset in call of Tls_Received, eventually directly in the context of <user>_RxIndication. */
      Tls_AppDataLenWaitingForRec[TConnId] += MaxRxLen;

      (void)Tls_VAd_Rx(TConnId, PlainTextDataPtr, MaxRxLen);
      /* The length forwarded to the user will be marked as ConsumedLen. The user has not necessarily really yet
      consumed data, data has to be stored for later usage. Buffer can/will be freed in call of Tls_Received. The length of the data is stored in Tls_Rl_DecryptedDataLen[TConnId]. */

      if(0 == Tls_AppDataLenWaitingForRec[TConnId])
      {
        /* The user has confirmed the data directly in its RxIndication. */
        /* do nothing, just continue */
      }
      else
      {
        /* The user has not confirmed the data yet but will do it in a later call of Tls_Received. */
      }
      retVal = E_OK;
      break;
    }
  case(TLS_REC_CONTENT_HS):  /* handshake data */
    {
      /* currently receiving handshake msg */
      ConsumedLen = Tls_VHs_Rx(TConnId, PlainTextDataPtr, MaxRxLen, 0);
      if(0 == ConsumedLen)
      {
        /* error, no length consumed */
        TLS_LOG_RL("Tls_VRl_RxPayload: error, no handshake data consumed");
      }
      else
      {
        retVal = E_OK;
      }

      break;
    }
  case(TLS_REC_CONTENT_CCS):  /* change cipher spec */
    {
      /* currently receiving change cipher spec msg */
      /* complete CCS message has a length of one byte */
      ConsumedLen = Tls_VCcs_Rx(TConnId, PlainTextDataPtr, MaxRxLen);
      if(MaxRxLen != ConsumedLen)
      {
        /* error, TLS protocol layer has to consume all data at once !
        this includes the case that '0' byte are consumed (as an error indication) */
      }
      else
      {
        retVal = E_OK;
      }
      break;
    }
  case(TLS_REC_CONTENT_A):  /* alarm data */
    {
      /* currently receiving alert msg */
      /* complete alert message has a length of two byte */
      ConsumedLen = Tls_VAl_Rx(TConnId, PlainTextDataPtr, MaxRxLen);
      if(0 == ConsumedLen)
      {
        /* error in alarm handling, alarm has to consume all data at once !
        this includes the case that '0' byte are consumed (as an error indication) */
      }
      else
      {
        retVal = E_OK;
      }
      break;
    }
#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
  case(TLS_REC_CONTENT_HB):  /* heartbeat data */
    {
      /* currently receiving heartbeat msg */
      ConsumedLen = Tls_VHb_Rx(TConnId, PlainTextDataPtr, MaxRxLen);
      if(0 == ConsumedLen)
      {
        /* error in heartbeat message, message has to consume all data at once !
        this includes the case that '0' byte are consumed (as an error indication) */
      }
      else
      {
        retVal = E_OK;
      }
      break;
    }
#endif
  default:
    {
      /* error, invalid content type */
    }
  } /* end of switch(Tls_RxProtocol[TConnId]) */

  return retVal;
}  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  Tls_VHs_Rx
 **********************************************************************************************************************/
/*! \brief      Receive handshake msg
 *  \param[in]  TConnId          TLS connection id
 *  \param[in]  DataPtr          pointer to the received (decrypted) data
 *  \param[in]  RxLen            data length that shall be received (total length of (several) handshake message(s)
 *  \param[in]  CurrentReadPos   Current read position inside the received data
 *  \return                      consumed length, (part of) RL header length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_Rx(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen,
    uint16 CurrentReadPos )
{
  uint16 RemLen;  /* remaining rx length */

  if((0 != CurrentReadPos) && (TLS_RX_BLOCKING_STATE_LOCKED == Tls_RxBlockingState[TConnId]))
  {
    Tls_RxBlockingState[TConnId] = TLS_RX_BLOCKING_STATE_UNLOCKING;
  }
  else
  {
    /* do nothing */
  }

  TLS_ASSERT(RxLen >= CurrentReadPos)

  RemLen = RxLen - CurrentReadPos;
  while((CurrentReadPos < RxLen) && (TLS_RX_BLOCKING_STATE_LOCKED != Tls_RxBlockingState[TConnId]))
  {
    uint16 LocCurrentReadPos = CurrentReadPos;

    /* read handshake message type */
    TLS_LOG_HS_DIV("Tls_VHs_Rx: reading HS type")
    Tls_Hs_HsType[TConnId] = DataPtr[CurrentReadPos];

    /* filter invalid handshake values */
    switch(Tls_Hs_HsType[TConnId])
    {
    case (TLS_HS_TYPE_HELLO_REQUEST):
    case (TLS_HS_TYPE_SERVER_HELLO):
    case (TLS_HS_TYPE_CERTIFICATE):
    case (TLS_HS_TYPE_CERT_STATUS):
    case (TLS_HS_TYPE_SERVER_KEY_EXCH):
    case (TLS_HS_TYPE_CERT_REQUEST):
    case (TLS_HS_TYPE_SERVER_HELLO_DONE):
    case (TLS_HS_TYPE_FINISHED):
      {
        /* valid value */
        break;
      }
    default:
      {
        /* error, handshake type does not have a valid value */
        Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
        return 0;
      }
    }

    /* go to next element */
    CurrentReadPos++;

    /* read handshake message length */
    TLS_LOG_HS_DIV("Tls_VHs_Rx: reading HS element length")
    /* copy three byte length */
    TLS_GET_UINT24(DataPtr, CurrentReadPos, Tls_Hs_Len[TConnId]);
    TLS_LOG_HS_DIV_3("Tls_VHs_Rx: HS element type is 0x%x and length is %d or 0x%x", Tls_Hs_HsType[TConnId], Tls_Hs_Len[TConnId], Tls_Hs_Len[TConnId])
    CurrentReadPos += TLS_HS_S_LEN_LEN;

    /* go to next part of message */
    if( (Tls_Hs_HsType[TConnId] != TLS_HS_TYPE_SERVER_HELLO_DONE) && (Tls_Hs_HsType[TConnId] != TLS_HS_TYPE_HELLO_REQUEST) )
    {
      /* in handshake message specific part, evaluate data in external function */
      uint16 ConsumedLen;
      Tls_Ul_CurrentElement[TConnId] = TLS_HS_S_IN_GENERIC_3;

      if( (uint16)(RxLen - CurrentReadPos) < Tls_Hs_Len[TConnId] )
      {
        /* error, invalid length value */
        return 0;
      }

      ConsumedLen = Tls_VHs_RxMsg(TConnId, &DataPtr[CurrentReadPos], (uint16)Tls_Hs_Len[TConnId]);
      if(0 != ConsumedLen)
      {
        CONST(uint8, TLS_CONST) Offs = TLS_HS_S_LEN_LEN +1;

        Tls_VHs_HashRxMsg(TConnId, &DataPtr[CurrentReadPos -Offs], (ConsumedLen +Offs));

        CurrentReadPos += ConsumedLen;
        if(CurrentReadPos == Tls_RxPlainTextFillLevel[TConnId])
        {
          Tls_HsContinueReadPos[TConnId] = 0;
        }
        else
        {
          Tls_HsContinueReadPos[TConnId] = CurrentReadPos;
        }
      }
      else
      {
        /* error */
        return 0;
      }
    }
    else
    {
      if(Tls_Hs_HsType[TConnId] == TLS_HS_TYPE_SERVER_HELLO_DONE)
      {
        if( (TLS_HS_STATE_CERT_REC <= Tls_HandShakeState[TConnId]) && (TLS_HS_STATE_HELLO_DONE_REC > Tls_HandShakeState[TConnId]) )
        {
          CONST(uint8, TLS_CONST) Offs = TLS_HS_S_LEN_LEN +1;
          /* TLS_HS_TYPE_SERVER_HELLO_DONE: there will be no payload, length of this handshake type is always '0' */
          Tls_HandShakeState[TConnId] = TLS_HS_STATE_HELLO_DONE_REC;
          Tls_VHs_HashRxMsg(TConnId, &DataPtr[CurrentReadPos -Offs], Offs);

          if(CurrentReadPos == Tls_RxPlainTextFillLevel[TConnId])
          {
            Tls_HsContinueReadPos[TConnId] = 0;
          }
          else
          {
            /* Error, there shouldn't be any received handshake message after server_hello_done, the next messages has to be sent by the client. */
            Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNEXP_MSG);
            return 0;
          }
        }
        else
        {
          /* error, handshake type does not have a valid value (sequence incorrect?) */
          Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNEXP_MSG);
          return 0;
        }
      }
      else
      {
        if( (TLS_HS_STATE_INIT == Tls_HandShakeState[TConnId]) || (TLS_HS_STATE_FINISHED_REC <= Tls_HandShakeState[TConnId]) )
        {
          /* TLS_HS_TYPE_HELLO_REQUEST: there will be no payload, length of this handshake type is always '0' */
          Tls_HandShakeState[TConnId] = TLS_HS_STATE_HELLO_REQUESTED;
          Tls_HsContinueReadPos[TConnId] = CurrentReadPos;
        }
        else
        {
          /* error, handshake type does not have a valid value (sequence incorrect?) */
          Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNEXP_MSG);
          return 0;
        }
      }
    }

    if( (RemLen == (RxLen - CurrentReadPos)) && ( (Tls_Hs_HsType[TConnId] == TLS_HS_TYPE_SERVER_HELLO_DONE) || (Tls_Hs_HsType[TConnId] == TLS_HS_TYPE_HELLO_REQUEST) ) )
    {
      /* error, no length was consumed */
      return 0;
    }

    RemLen -= (CurrentReadPos - LocCurrentReadPos);
  }

  if(TLS_RX_BLOCKING_STATE_UNLOCKING == Tls_RxBlockingState[TConnId])
  {
    if(RemLen != 0)
    {
      Tls_RxBlockingState[TConnId] = TLS_RX_BLOCKING_STATE_LOCKED;
    }
    else
    {
      Tls_RxPlainTextFillLevel[TConnId] = 0;
      Tls_Rl_TotalPayloadLen[TConnId] = 0;
      Tls_RxBlockingState[TConnId] = TLS_RX_BLOCKING_STATE_NONE;
    }
  }

  /* return consumed length */
  return CurrentReadPos;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VAl_Rx
 **********************************************************************************************************************/
/*! \brief      Receive alert msg
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \param[in]  RxLen          data length that shall be received
 *  \return                    consumed length, (part of) RL header length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VAl_Rx(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen )
{
  /* check rx length */
  if(TLS_AL_MSG_LEN < RxLen)
  {
    /* error, this is no complete alert message */
    return 0;
  }
  TLS_LOG_ALERT("Tls_VAl_Rx: Alert received ! ------------------")

  /* read alert level */
  {
    Tls_Al_Level[TConnId] = DataPtr[0];
    TLS_LOG_ALERT_1("Tls_VAl_Rx: Alert level is %d (1 = warning, 2 = fatal)", Tls_Al_Level[TConnId])
    Tls_Ul_CurrentElement[TConnId] = TLS_AL_IN_TYPE;
  }

  /* read alert type */
  {
    Tls_Al_Type[TConnId] = DataPtr[1];
    TLS_LOG_ALERT_1("Tls_VAl_Rx: Alert type is %d", Tls_Al_Type[TConnId])
    Tls_Ul_CurrentElement[TConnId] = TLS_UL_IN_FIRST_ELEMENT;
  }

  if(TLS_ALERT_DESC_CLOSE_NOTIFY == Tls_Al_Type[TConnId])
  {
    /* trigger closure if not already done */
    if(Tls_ConnState[TConnId] == TLS_STATE_TCP_CLOSING_TLS_CLOSED)
    {
      /* don't forward this to user, will be notified when TCP indicates 'closed' */
    }
    else
    {
      /* Don't forward the event to TCPIP. Usally the TLS message 'close_notify' is received together with a TCP FIN,
         so the information that the other side wants to close the connection is already available in TCP. */
    }
  }
  else
  {
    if(TLS_ALERT_LEVEL_WARNING == Tls_Al_Level[TConnId])
    {
      /* send close notify, then close connection */
      Tls_PendingAlertId[TConnId]       = TLS_ALERT_DESC_CLOSE_NOTIFY;
      Tls_PendingAlertLevel[TConnId]    = TLS_ALERT_LEVEL_WARNING;
      (void)Tls_VAl_SendAlert(TConnId, TLS_ALERT_LEVEL_WARNING, TLS_ALERT_DESC_CLOSE_NOTIFY);
    }
    else
    {
      /* alert level is 'fatal', so close connection immediatelly */
      /* Don't forward the event to TCPIP. Usally a TLS alert message with level 'fatal' is received together with a TCP FIN,
         so the information that the other side wants to close the connection is already available in TCP. */
    }

#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
    if(TLS_HS_STATE_CONN_ESTABLISHED != Tls_HandShakeState[TConnId])
    {
      /*  the server terminates the TLS handshake with an error alert while the TLS handshake was ongoing */
      Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[TConnId], TLS_E_SERVER_TERMINATED, TRUE);
    }
#endif
  }

  /* close connection, no matter if the alert level was 'fatal' or 'warning' */
  (void)Tls_TcpClose(Tls_VCfgGetTlsConn2SockIdMap()[TConnId]);

  /* return consumed length */
  return TLS_AL_MSG_LEN;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VAd_Rx
 **********************************************************************************************************************/
/*! \brief      Receive application data msg
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \param[in]  RxLen          data length that shall be received
 *  \return                    consumed length, (part of) RL header length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VAd_Rx(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen )
{
  TcpIp_SocketIdType SocketId = Tls_VCfgGetTlsConn2SockIdMap()[TConnId];  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */

  TcpIp_Tcp_TlsRxIndication(SocketId, DataPtr, RxLen);

  /* return consumed length */
  return RxLen;
}

/***********************************************************************************************************************
 *  Tls_VCcs_Rx
 **********************************************************************************************************************/
/*! \brief      Receive change cipher spec msg
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \param[in]  RxLen          data length that shall be received
 *  \return                    consumed length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VCcs_Rx(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen )
{
  /* indicate complete message to upper layer / application */
  /* source is the same as the destination used for connecting to the server */

  uint16 retLen;

  if((1 != DataPtr[0]) || (RxLen != 1))
  {
    /* error, wrong content in this message or wrong message length */
    retLen = 0;
  }
  else
  {
    Tls_HandShakeState[TConnId] = TLS_HS_STATE_CCS_REC;

    /* switch active security parameters and keyblock material for Rx: */
    {
      Tls_SecurityParametersType *TmpSecPtr;
      TmpSecPtr                         = Tls_SecParamRxActivePtr[TConnId];
      Tls_SecParamRxActivePtr[TConnId]  = Tls_SecParamRxPassivePtr[TConnId];
      Tls_SecParamRxPassivePtr[TConnId] = TmpSecPtr;
    }
    {
      Tls_KeyMaterialType *TmpKeyPtr;
      TmpKeyPtr                            = Tls_KeyMatBlockRxActivePtr[TConnId];
      Tls_KeyMatBlockRxActivePtr[TConnId]  = Tls_KeyMatBlockRxPassivePtr[TConnId];
      Tls_KeyMatBlockRxPassivePtr[TConnId] = TmpKeyPtr;
    }
    Tls_Rl_CurrentPart[TConnId] = TLS_RL_IN_HDR;

    Tls_VSeqNumInit(&Tls_RxSeqNum[TConnId]);

    retLen = RxLen;
  }

  /* return consumed length (=1) */
  return retLen;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VHs_RxMsg
 **********************************************************************************************************************/
/*! \brief      Receive handshake msg, starting after length value
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \param[in]  RxLen          data length that shall be received (complete hs msg without type and length field)
 *  \return                    consumed length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxMsg(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen )
{
  uint16 LocReadPos;
  uint16 ConsumedLen;

  TLS_LOG_HS_DIV("Tls_VHs_RxMsg: reading HS msg content")

  LocReadPos = 0;

  switch(Tls_Hs_HsType[TConnId])
  {
  case(TLS_HS_TYPE_SERVER_HELLO):
    {
      if(TLS_HS_STATE_HELLO_SENT == Tls_HandShakeState[TConnId])
      {
        ConsumedLen = Tls_VHs_RxServerHello(TConnId, &DataPtr[LocReadPos], (RxLen - LocReadPos));
      }
      else
      {
        /* Error, unexpeced handshake message, this message is only expeted to be received after sending of the client_hello message. */
        ConsumedLen = 0;  /* error */
      }
      break;
    }
  case(TLS_HS_TYPE_CERTIFICATE):
    {
      if( (TLS_HS_STATE_HELLO_REC == Tls_HandShakeState[TConnId]) )
      {
        ConsumedLen = Tls_VHs_RxServerCert(TConnId, &DataPtr[LocReadPos], (RxLen - LocReadPos));
      }
      else
      {
        /* Error, unexpeced handshake message, this message is only expeted to be received after the server_hello message. */
        ConsumedLen = 0;  /* error */
      }
      break;
    }
  case(TLS_HS_TYPE_SERVER_KEY_EXCH):
    {
      if( (TLS_HS_STATE_CERT_REC == Tls_HandShakeState[TConnId]) )
      {
        ConsumedLen = Tls_VHs_RxServerKeyExchange(TConnId, &DataPtr[LocReadPos]);
      }
      else
      {
        ConsumedLen = 0;  /* error */
      }
      break;
    }
  case(TLS_HS_TYPE_CERT_REQUEST):
    {
      /* If the support for client certificates is disabled in the configuration, receiving this message results in an error. */
#if (TLS_SUPPORT_CLIENT_CERT == STD_ON)
      /* evaluate certificate request */
      if( (TLS_HS_STATE_CERT_REC <= Tls_HandShakeState[TConnId]) && (TLS_HS_STATE_HELLO_DONE_REC > Tls_HandShakeState[TConnId]) )
      {
        ConsumedLen = Tls_VHs_RxCertRequest(TConnId, &DataPtr[LocReadPos]);
      }
      else
#endif
      {
        ConsumedLen = 0;  /* error */
      }
      break;
    }
  case(TLS_HS_TYPE_FINISHED):
    {
      if( (TLS_HS_STATE_CCS_REC == Tls_HandShakeState[TConnId]) )
      {
        ConsumedLen = Tls_VHs_RxServerFinished(TConnId, &DataPtr[LocReadPos]);
      }
      else
      {
        ConsumedLen = 0;  /* error */
      }
      break;
    }
#if (TLS_SUPPORT_OCSP == STD_ON)
  case(TLS_HS_TYPE_CERT_STATUS):
    {
      if(Tls_OcspWorkingMode[TConnId] != TLS_OCSP_MODE_OFF)
      {
        if( (TLS_HS_STATE_CERT_REC == Tls_HandShakeState[TConnId]) )
        {
          ConsumedLen = Tls_VHs_RxServerCertStatus(TConnId, &DataPtr[LocReadPos]);
          if(0 == ConsumedLen)
          {
            Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_BAD_CERT_STATUS_RESP);
          }
        }
        else
        {
          ConsumedLen = 0;  /* error */
        }
      }
      else
      {
        /* The message 'server hello' did not contain an extension status_request, so it is not allowed to receive this kind of message now */
        ConsumedLen = 0;  /* error */
      }
      break;
    }
#endif
  default:
    {
      /* error, invalid or not supported handshake type */
      Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNEXP_MSG);
      return 0;
    }
  }

  /* TLS_HS_TYPE_SERVER_HELLO_DONE is already filtered in previous function */
  if(0 == ConsumedLen)
  {
    /* error */
    return 0;
  }

  /* some special handlings */
  if(TLS_HS_TYPE_FINISHED == Tls_Hs_HsType[TConnId])
  {
    Tls_HandShakeState[TConnId] = TLS_HS_STATE_FINISHED_REC;
  }
  else if(TLS_HS_TYPE_CERT_REQUEST == Tls_Hs_HsType[TConnId])
  {
    Tls_HandShakeState[TConnId] = TLS_HS_STATE_CERT_REQUESTED;
  }
  else
  {
    /* do nothing */
  }

  /* return overall consumed length */
  return ConsumedLen;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  Tls_VHs_RxServerHello
 **********************************************************************************************************************/
/*! \brief      Receive handshake msg server_hello
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \param[in]  RxLen          data length that shall be received
 *  \return                    consumed length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerHello(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen )
{
  uint16  LocReadPos       = 0;
  boolean LocInternalError = FALSE;

  /*
    -  2 byte: version
    - 32 byte: server random
    -  1 byte: session id length
    - 0..32 byte: session id
    -  2 byte: cipher suite
    -  1 byte: compression method
    - xx byte: extensions
  */

  /* check minimum for RxLen */
  if(RxLen < 38)  /* sum of all obligatory elements */
  {
    return 0;  /* error, message too short */
  }

  /* check TLS protocol version */
  {
    if(TLS_MAJOR_VER != DataPtr[LocReadPos + 0])
    {
      return 0;  /* error */
    }
    switch(DataPtr[LocReadPos + 1])
    {
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    case TLS_MINOR_VER_TLS12:
      {
        /* TLS 1.2 */
        Tls_ProtVer[TConnId]   = TLS_PROT_VER_TLS12;
        Tls_HsHashAlg[TConnId] = TLS_HS_HASH_ALG_SHA256;
        Tls_HsHashLen[TConnId] = TLS_HASH_SHA256_LEN;
        break;
      }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
    case TLS_MINOR_VER_TLS10:
      {
        /* TLS 1.0 */
        Tls_ProtVer[TConnId]   = TLS_PROT_VER_TLS10;
        Tls_HsHashAlg[TConnId] = TLS_HS_HASH_ALG_MD5SHA1;
        Tls_HsHashLen[TConnId] = TLS_HASH_MD5_LEN + TLS_HASH_SHA1_LEN;
        break;
      }
#endif
    default:
      {
        /* error, server has chosen an invalid (not supported and not offered) TLS protocol version */
        Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_HANDSHAKE_FAILURE);
        return 0;  /* error */
      }
    }
    LocReadPos += TLS_HS_S_HELLO_VERSION_LEN;
  }

  /* read server random */
  {
    /* copy data from received message */
    IpBase_Copy((IpBase_CopyDataType*)(&Tls_SecParamTxPassivePtr[TConnId]->server_random[0]), (const IpBase_CopyDataType*)(&DataPtr[LocReadPos]), TLS_HS_S_HELLO_RANDOM_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
    LocReadPos += TLS_HS_S_HELLO_RANDOM_LEN;
  }

  /* read session id length and session id */
  {
    Tls_SessionIdLen[TConnId] = DataPtr[LocReadPos];
    if(0 != Tls_SessionIdLen[TConnId])
    {
      /* copy session id */
      IpBase_Copy( (IpBase_CopyDataType*)(&Tls_SessionId[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
                   (const IpBase_CopyDataType*)(&DataPtr[LocReadPos +TLS_HS_S_HELLO_SID_LEN_LEN]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
                   Tls_SessionIdLen[TConnId] );
    }
    LocReadPos += (Tls_SessionIdLen[TConnId] + TLS_HS_S_HELLO_SID_LEN_LEN);
  }

  /* read cipher suite
     -> is chosen by the server */
  {
    uint16 ActiveCipherSuite;
    ActiveCipherSuite = (uint16)(((uint16)(DataPtr[LocReadPos +0]) << 8) + DataPtr[LocReadPos +1]);

    /* check if server has chosen one of the offered cipher suites */
    {
      uint16 SuiteIndex;

      for(SuiteIndex=0; SuiteIndex < Tls_VCfgGetNumSuppCipherSuites(); SuiteIndex++)
      {
        if(Tls_VCfgGetSuppCipherSuites(SuiteIndex) == ActiveCipherSuite)
        {
          break;
        }
      }
      if(SuiteIndex == Tls_VCfgGetNumSuppCipherSuites())
      {
        /* error, server has chosen an invalid (not supported and not offered) cipher suite */
        Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_HANDSHAKE_FAILURE);
        return 0;
      }
    }

    /* store the chosen cypher suite */
    Tls_SecParamTxPassivePtr[TConnId]->cipher_suite = ActiveCipherSuite;
    Tls_SecParamRxPassivePtr[TConnId]->cipher_suite = ActiveCipherSuite;

    switch(ActiveCipherSuite)
    {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
    case (TLS_RSA_WITH_NULL_SHA):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_RSA;
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_NULL;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = 0;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = 0;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA1;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA1_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_STREAM;
        break;
      }
    case (TLS_RSA_WITH_AES_128_CBC_SHA):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_RSA;
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_AES128_CBC;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = TLS_AES128_KEY_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = TLS_AES_BLOCK_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA1;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA1_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_BLOCK;
        break;
      }
    case (TLS_RSA_WITH_NULL_SHA256):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_RSA;
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_NULL;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = 0;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = 0;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA256;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA256_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_STREAM;
        break;
      }
    case (TLS_RSA_WITH_AES_128_CBC_SHA256):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_RSA;
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_AES128_CBC;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = TLS_AES128_KEY_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = TLS_AES_BLOCK_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA256;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA256_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_BLOCK;
        break;
      }
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
    case (TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_ECDH;
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_AES128_CBC;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = TLS_AES128_KEY_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = TLS_AES_BLOCK_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA1;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA1_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_BLOCK;
        break;
      }
    case (TLS_ECDH_ECDSA_WITH_NULL_SHA):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_ECDH;
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_NULL;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = 0;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = 0;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA1;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA1_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_STREAM;
        break;
      }
    case (TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_ECDH;
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_AES128_CBC;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = TLS_AES128_KEY_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = TLS_AES_BLOCK_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA256;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA256_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_BLOCK;
        break;
      }
    case (TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_ECDH;  /* = TLS_KEY_EXCH_TYPE_ECDHE; */
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_AES128_CBC;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = TLS_AES128_KEY_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = TLS_AES_BLOCK_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA1;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA1_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_BLOCK;
        break;
      }
    case (TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_ECDH;  /* = TLS_KEY_EXCH_TYPE_ECDHE; */
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_AES128_CBC;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = TLS_AES128_KEY_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = TLS_AES_BLOCK_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_HMAC_SHA256;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = TLS_HASH_SHA256_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_BLOCK;
        break;
      }
#if (TLS_SUPPORT_AES_GCM == STD_ON)
    case (TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256):
      {
        /* key exchange type */
        Tls_Hs_KeyExchType[TConnId]                              = TLS_KEY_EXCH_TYPE_ECDH;  /* = TLS_KEY_EXCH_TYPE_ECDHE; */
        /* cipher algorithm / MAC algorithm and length for Rx and Tx: */
        Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_AES128_GCM;
        Tls_SecParamRxPassivePtr[TConnId]->enc_key_length        = TLS_AES128_KEY_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->enc_block_length      = TLS_AES_BLOCK_LEN;
        Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_AEAD_SHA256;
        Tls_SecParamRxPassivePtr[TConnId]->mac_length            = 0;
        Tls_SecParamRxPassivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_AEAD;

        break;
      }
#endif  /* AES_GCM */
#endif  /* ECDH */
    default:
      {
        /* error, server has chosen an invalid (not supported and not offered) cipher suite */
        Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_HANDSHAKE_FAILURE);
        return 0;
      }
    }

    /* copy security parameters from Rx to Tx */
    Tls_SecParamTxPassivePtr[TConnId]->bulk_cipher_algorithm = Tls_SecParamRxPassivePtr[TConnId]->bulk_cipher_algorithm;
    Tls_SecParamTxPassivePtr[TConnId]->enc_key_length        = Tls_SecParamRxPassivePtr[TConnId]->enc_key_length;
    Tls_SecParamTxPassivePtr[TConnId]->enc_block_length      = Tls_SecParamRxPassivePtr[TConnId]->enc_block_length;
    Tls_SecParamTxPassivePtr[TConnId]->mac_algorithm         = Tls_SecParamRxPassivePtr[TConnId]->mac_algorithm;
    Tls_SecParamTxPassivePtr[TConnId]->mac_length            = Tls_SecParamRxPassivePtr[TConnId]->mac_length;
    Tls_SecParamTxPassivePtr[TConnId]->cipher_type           = Tls_SecParamRxPassivePtr[TConnId]->cipher_type;

    LocReadPos += TLS_HS_S_HELLO_CIPHER_S_LEN;
  }

#if ((TLS_SUPPORT_PROT_VER_TLS10 == STD_ON) && (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON))
  /* Now that it is decided that TLS1.0 is used, copy partly calculated hashes from ws2 to ws1. */
  if(Tls_HsHashAlg[TConnId] == TLS_HS_HASH_ALG_MD5SHA1)
  {
    /* Copy sha1 and md5 hashes from ws2 to ws1. */
    IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                 (const IpBase_CopyDataType*)(&Tls_WorkSpaceShaX_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                 (uint16)sizeof(eslt_WorkSpaceSHA256) );
    IpBase_Copy( (IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs1[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                 (const IpBase_CopyDataType*)(&Tls_WorkSpaceMd5_Hs2[TConnId]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */
                 (uint16)sizeof(eslt_WorkSpaceMD5) );
  }
#endif

  /* read compression method
     -> is chosen by the server */
  {
    uint8 CompAlg = DataPtr[LocReadPos];
    if(TLS_COMPR_ALGORITHM_NULL == CompAlg)
    {
      /* compression algorithm is 'NULL', this is the only supported algorithm */
      /* -> ok, nothing to do */
    }
    else
    {
      /* error, server has chosen an invalid (not supported and not offered) compression algorithm */
      Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_HANDSHAKE_FAILURE);
      return 0;
    }
    LocReadPos += TLS_HS_S_HELLO_COMP_M_LEN;
  }

  /* read hello extensions */
  if(RxLen != LocReadPos)
  {
    uint16 ExtLen;
    uint16 CurExtLen;
    uint16 RemExtLen;
    uint16 CurExtType;
    IPBASE_GET_UINT16(DataPtr, LocReadPos, ExtLen);
    LocReadPos += 2;

    RemExtLen = ExtLen;
    while(0 != RemExtLen)
    {
      /* each extension starts with 2 byte type field and 2 byte length field, followed by some bytes of type specific data */
      if(RemExtLen < TLS_HS_HELLO_OFFSET_EXTENSION_CONTENT)
      {
        LocInternalError = TRUE;  /* error, extension is too short (type and length need 4 bytes at least) */
        break;
      }
      IPBASE_GET_UINT16(DataPtr,  LocReadPos, CurExtType);
      IPBASE_GET_UINT16(DataPtr, (LocReadPos + TLS_HS_HELLO_EXTENSION_TYPE_LEN), CurExtLen);
      if(CurExtLen > (RemExtLen - TLS_HS_HELLO_OFFSET_EXTENSION_CONTENT))
      {
        LocInternalError = TRUE;  /* error, remaining data is too short */
        break;  /* PRQA S 0771 */ /* MD_TLS_TCP_14.6_0771_c */
      }

      /* now handle the extension data */
      switch(CurExtType)
      {
      case TLS_HS_HELLO_EXT_EC_POINT_FORMATS:  /* ec_point_formates */
        {
          uint8 PointIndex;
          uint8 NumPointFormats;
          uint8 *PointFormPtr;
          uint8 *ExtensionContentPtr = &(DataPtr[LocReadPos + TLS_HS_HELLO_OFFSET_EXTENSION_CONTENT]);

          NumPointFormats = ExtensionContentPtr[0];
          PointFormPtr     = &ExtensionContentPtr[1];
          for(PointIndex = 0; PointIndex < NumPointFormats; PointIndex++)
          {
            if(TLS_HELLO_EXT_ECC_POINT_FORMAT_UNCOMPRESSED == PointFormPtr[PointIndex])  /* uncompressed point format */
            {
              /* supported point formate found (only uncompressed format is supported) */
              break;
            }
          }
          if(PointIndex == NumPointFormats)
          {
            /* error, no supported point format found */
          }
          break;
        }
#if (TLS_SUPPORT_OCSP == STD_ON)
      case TLS_HS_HELLO_EXT_OCSP_STATUS_REQUEST:  /* status_request (OCSP) */
        {
          /* this is an empty extension, only evaluate the excistence of this extension in the server hello message */
          Tls_OcspWorkingMode[TConnId] = TLS_OCSP_MODE_SUPPORT_V1;
          break;
        }
      case TLS_HS_HELLO_EXT_OCSP_STATUS_REQUEST_V2:  /* status_request_V2 (OCSP) */
        {
          /* this is an empty extension, only evaluate the excistence of this extension in the server hello message */
          Tls_OcspWorkingMode[TConnId] = TLS_OCSP_MODE_SUPPORT_V1_V2;
          break;
        }
#endif
#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
      case TLS_HS_HELLO_EXT_HEARTBEAT:
        {
          uint8 *ExtensionContentPtr = &(DataPtr[LocReadPos + TLS_HS_HELLO_OFFSET_EXTENSION_CONTENT]);
          if(CurExtLen != 1)  /* length of the heartbeat extension is always '1' */
          {
            /* error, invalid length for this extension */
            LocInternalError = TRUE;
          }
          else
          {
            Tls_HeartbeatModePeer[TConnId] = ExtensionContentPtr[0];  /* TLS_HB_PEER_NOT_ALLOWED_TO_SEND or TLS_HB_PEER_ALLOWED_TO_SEND */
            if(Tls_HeartbeatModePeer[TConnId] == TLS_HB_PEER_NOT_ALLOWED_TO_SEND)
            {
              /* The server does not allow us to send heartbeat requests, but this is the (only) mode we want to support.
              So we won't send any heartbeat messages since the peer did not allow us to send heartbeat requests. */
              Tls_HeartbeatModeOwn[TConnId] = TLS_HB_MODE_NONE;
              TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_INTERNAL_FUNCTION, TLS_EVENT_PEER_ALLOWS_SENDING_HEARTBEAT_MSG, TLS_EVENT_STATUS_FAILED)  /* report event */
              TLS_LOG_HS_DIV("Tls_VHs_RxServerHello: Server does not allow us to send heartbeat requests");
            }
          }
          break;
        }
#endif
      default:
        {
          /* unknown extension type -> skip this extension */
          break;
        }
      }

      /* go to next extension */
      RemExtLen  -= (CurExtLen + TLS_HS_HELLO_OFFSET_EXTENSION_CONTENT);
      LocReadPos += (CurExtLen + TLS_HS_HELLO_OFFSET_EXTENSION_CONTENT);
    }

    if(LocInternalError == TRUE)
    {
      /* internal error */
      Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INT_ERROR);
      return 0;
    }
  }  /* end of reading hello extensions */

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
  if((Tls_HeartbeatModeOwn[TConnId] != TLS_HB_MODE_NONE) && (Tls_HeartbeatModePeer[TConnId] == TLS_HB_MODE_NONE))
  {
    /* the server did not send a heartbeat extension, so we are not allowed to send any heartbeat messages */
    Tls_HeartbeatModeOwn[TConnId] = TLS_HB_MODE_NONE;  /* switch off sending heartbeats */
  }
#endif

  /* ----------------------------------------------------- */
  /* handshake message is completed, go on to the next one */

  Tls_Ul_CurrentElement[TConnId] = TLS_HS_S_IN_TYPE;
  Tls_HandShakeState[TConnId] = TLS_HS_STATE_HELLO_REC;
  TLS_LOG_HS_DIV("Tls_VHs_RxServerHello: finished reading server_hello message");

  return LocReadPos;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VHs_RxServerCert
 **********************************************************************************************************************/
/*! \brief      Receive handshake msg server_certificate
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \param[in]  RxLen          data length that shall be received
 *  \return                    consumed length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerCert(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
    uint16 RxLen )
{
  uint16  LocReadPos;
  uint16  AllCertsRemLen;
#if (TLS_SUPPORT_CRL == STD_ON)
  boolean MoreThanOneCert;
#endif
  uint32  CurrentCertLen;
  uint8  *CurrentCertPtr;

  Tls_RxCertDescCurrentPtr[TConnId]  = &Tls_RxCertDescs[ TConnId *2];
  Tls_RxCertDescChildPtr[TConnId]    = &Tls_RxCertDescs[(TConnId *2) +1];

  LocReadPos     = 0;
  AllCertsRemLen = RxLen;

#if (TLS_SUPPORT_OCSP == STD_ON)
  Tls_Ocsp_CertChainLen[TConnId] = 0;  /* init */
#endif

  /*
  - 3 byte total length of all included certificate(s)
  - certificate:
  - 3 byte length of current certificate
  - cert
  */

  /* check minimum length needed to read the message length */
  if(RxLen < TLS_HS_S_CERT_CERTS_LEN_LEN)
  {
    return 0;  /* error, the message is too short */
  }

  /* ---------------------------------------------------------------------------------------- */
  if(TLS_HS_S_CERT_IN_ALL_CERTS_LEN == Tls_Ul_CurrentElement[TConnId])
  {
    /* read all certs length */
    TLS_GET_UINT24(DataPtr, LocReadPos, Tls_Hs_AllCertsRemLen[TConnId]);

    /* currently (in this implementation) all certificates have to be received inside one recordlayer message */
    if(RxLen != (Tls_Hs_AllCertsRemLen[TConnId] + TLS_HS_S_CERT_CERTS_LEN_LEN))
    {
      /* this use case is currently not supported -> cancel handshake */
      return 0;
    }
    Tls_Ul_CurrentElement[TConnId]    = TLS_HS_S_CERT_IN_CERT_DATA;
    LocReadPos                       += TLS_HS_S_CERT_CERTS_LEN_LEN;
    AllCertsRemLen                   -= TLS_HS_S_CERT_CERTS_LEN_LEN;
    Tls_HsServerCertChainPtr[TConnId] = &DataPtr[TLS_HS_S_CERT_CERTS_LEN_LEN];
    Tls_HsServerCertChainLen[TConnId] = Tls_Hs_AllCertsRemLen[TConnId];
  }
  /* ---------------------------------------------------------------------------------------- */
  if(TLS_HS_S_CERT_IN_CERT_DATA == Tls_Ul_CurrentElement[TConnId])
  {
    /* parse the (first) received certificate ------------------------------------------------------------ */
    uint8                   *LocDataPtr;
    CONST(uint32, TLS_CONST) CertMinContent = ( TLS_CERT_ELE_CERT_CH1 | TLS_CERT_ELE_SIGNED_CERT | TLS_CERT_ELE_ISSUER |
#if (TLS_SUPPORT_CRL == STD_ON)
      TLS_CERT_ELE_SERIAL_NUMBER |
#endif
      TLS_CERT_ELE_VALID_NOT_BEFORE | TLS_CERT_ELE_VALID_NOT_AFTER | TLS_CERT_ELE_SUBJECT |
      TLS_CERT_ELE_KEY_ALG_ID | TLS_CERT_ELE_CERT_SIGN_ALG_ID | TLS_CERT_ELE_CERT_SIGNATURE );
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
    CONST(uint32, TLS_CONST) CertMinContentRsa  = ( TLS_CERT_ELE_KEY_RSA_MODULUS | TLS_CERT_ELE_KEY_RSA_PUB_EXP );
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
    CONST(uint32, TLS_CONST) CertMinContentEcdh = ( TLS_CERT_ELE_KEY_ECC_CURVE | TLS_CERT_ELE_KEY_ECC_PUB_KEY );
#endif
    uint8                    CertHashElement;

    /* read length of first certificate */
    TLS_GET_UINT24(DataPtr, LocReadPos, CurrentCertLen);
    LocDataPtr     = &DataPtr[LocReadPos + TLS_HS_S_TLS_CERT_LEN_LEN];
    CurrentCertPtr = LocDataPtr;
#if (TLS_SUPPORT_OCSP == STD_ON)
  Tls_Ocsp_CertChainLen[TConnId] = 1;  /* first cert */
  TLS_LOG_HS_DIV_1("Tls_VHs_RxServerCert: cert chain length is now %d", Tls_Ocsp_CertChainLen[TConnId]);
#endif

#if (TLS_SUPPORT_CRL == STD_ON)
    /* check if there is a single certificate or a certificate chain */
    if((CurrentCertLen + TLS_HS_S_TLS_CERT_LEN_LEN) != Tls_Hs_AllCertsRemLen[TConnId])
    {
      MoreThanOneCert = TRUE; /* more than one cert in this message */
    }
    else
    {
      MoreThanOneCert = FALSE;
    }
#endif
    /* parse received cert */
    CertHashElement = TLS_HASH_INFO_ISSUER;
    if(E_OK != Tls_VParseServerCert(Tls_RxCertDescCurrentPtr[TConnId], CertHashElement, LocDataPtr, (uint16)CurrentCertLen))
    {
      /* parsing the certificate failed */
      return 0;
    }

    if( (Tls_RxCertDescCurrentPtr[TConnId]->CertFoundElements & CertMinContent) != CertMinContent )
    {
      /* not all necessary information found in the certificate */
      return 0;
    }

#if (TLS_SUPPORT_OCSP == STD_ON)
    /* DO NOT COPY PUB KEY. Key will already be stored in standard use case */
#endif

#if (TLS_SUPPORT_REAL_TIME_CLOCK == STD_ON)
    /* check validity (time) of received certificate */
    if(E_OK != Tls_VHs_CheckCertTimeValidity(LocDataPtr, Tls_RxCertDescCurrentPtr[TConnId]))
    {
      /* certificate is out of date, cancel handshake */
      /* send alert and close connection */
      Tls_Rl_CurrentPart[TConnId]    = TLS_RL_IN_HDR;
      Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_CERT_EXPIRED);
#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
      Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[TConnId], TLS_E_SERVER_CERT_EXPIRED, TRUE);  /* server cert is expired */
#endif
      return 0;
    }
#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
    else
    {
      Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[TConnId], TLS_E_SERVER_CERT_EXPIRED, FALSE);  /* server cert is not expired */
    }
#endif
#endif
      /* TLS_SUPPORT_REAL_TIME_CLOCK */

    switch(Tls_Hs_KeyExchType[TConnId])
    {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
    case(TLS_KEY_EXCH_TYPE_RSA):
      {
        /* Check that all necessary RSA key elements could be found in the certificate */
        if((Tls_RxCertDescCurrentPtr[TConnId]->CertFoundElements & CertMinContentRsa) != CertMinContentRsa)
        {
          /* error */
          return 0;
        }

        /* copy (RSA) key info from this cert (needed for key exchange) -------------------------------------- */
        /* copy RSA data */
        {
          TLS_LOG_HS_DIV("Tls_VHs_RxServerCert: all server_certificate elements for RSA found !");

          /* ok, copy key info */

          /* cert is always in one single linear buffer inside the TLS module (because of messages authentication check) */

          /* copy RSA mod and exp key information from the first certificate */
          if( (Tls_VCfgGetMaxServerCertKeySizeByte() >= Tls_RxCertDescCurrentPtr[TConnId]->KeyMain.ContLen) &&
            (Tls_VCfgGetMaxServerCertRsaExpSizeByte() >= Tls_RxCertDescCurrentPtr[TConnId]->KeyExp.ContLen) )
          {
            /* copy length and modulus */
            Tls_RsaKeyLen[TConnId] = (uint16)(Tls_RxCertDescCurrentPtr[TConnId]->KeyMain.ContLen);
            IpBase_Copy( (IpBase_CopyDataType*)((uint8 *)(&(Tls_ServEncKey[TConnId * Tls_VCfgGetMaxServerCertKeySizeByte()]))),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
              (const IpBase_CopyDataType*)(&(LocDataPtr[Tls_RxCertDescCurrentPtr[TConnId]->KeyMain.ContOffs])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
              (uint16)(Tls_RxCertDescCurrentPtr[TConnId]->KeyMain.ContLen) );

            /* copy length and exponent */
            Tls_RsaExpLen[TConnId] = (uint8)(Tls_RxCertDescCurrentPtr[TConnId]->KeyExp.ContLen);
            IpBase_Copy( (IpBase_CopyDataType*)(&(Tls_RsaExp[TConnId])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
              (const IpBase_CopyDataType*)(&(LocDataPtr[Tls_RxCertDescCurrentPtr[TConnId]->KeyExp.ContOffs])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
              (uint16)(Tls_RxCertDescCurrentPtr[TConnId]->KeyExp.ContLen) );
          }
          else
          {
            /* error */
            Tls_RsaKeyLen[TConnId] = 0;
            Tls_RsaExpLen[TConnId] = 0;
            return 0;
          }
        }
        break;
      }
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
    case(TLS_KEY_EXCH_TYPE_ECDH):
      {
        if((Tls_RxCertDescCurrentPtr[TConnId]->CertFoundElements & CertMinContentEcdh) != CertMinContentEcdh)
        {
          /* error */
          return 0;
        }

        /* copy ECDH data */
        /* store server public key (uncompressed) */
        {
          uint8 *DestPtr = &Tls_ServEncKey[Tls_VCfgGetMaxServerCertKeySizeByte() * TConnId];
          uint8 CompressionFormat = LocDataPtr[Tls_RxCertDescCurrentPtr[TConnId]->KeyMain.ContOffs];
          if(TLS_ECC_PUB_KEY_POINT_FORMAT_UNCOMPRESSED != CompressionFormat)
          {
            /* error: type not supported by this implementation */
            return 0;
          }
          /* copy length and pub key */
          Tls_ServEncKeyLen[TConnId] = (uint16)(Tls_RxCertDescCurrentPtr[TConnId]->KeyMain.ContLen -1);
          IpBase_Copy( (IpBase_CopyDataType*)(DestPtr),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
            (const IpBase_CopyDataType*)(&(LocDataPtr[Tls_RxCertDescCurrentPtr[TConnId]->KeyMain.ContOffs])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
            (uint16)(Tls_RxCertDescCurrentPtr[TConnId]->KeyMain.ContLen) );
        }
        break;
      }
#endif
    default:
      {
        /* error: not supported type */
        return 0;
      }
    }  /* switch */

    /* store current cert pointer */
    Tls_RxCertCurrentPtr[TConnId] = LocDataPtr;

    /* go to next cert */
    AllCertsRemLen -= (uint16)(CurrentCertLen + TLS_HS_S_TLS_CERT_LEN_LEN);
    LocReadPos     += (uint16)(CurrentCertLen + TLS_HS_S_TLS_CERT_LEN_LEN);

    /* check if there are further certificates (a chain) -> while(...) ----------------------------------- */
    {
      /* check the received certificate chain for internally being valid */

      /* store all relevant context information and set crypto-flag */
      Tls_RxBlockingState[TConnId]   = TLS_RX_BLOCKING_STATE_LOCKED;
      Tls_RxCertChildPtr[TConnId]    = CurrentCertPtr;
      Tls_CryptoCalcDataPtr[TConnId] = &DataPtr[LocReadPos];
      Tls_CryptoCalcDataLen[TConnId] = AllCertsRemLen;

      Tls_HsCurrDataPtr[TConnId]     = LocDataPtr;

#if (TLS_SUPPORT_CRL == STD_ON)
      /* trigger checking if the server cert is invalidated by a CRL */
      Tls_CrlServerCertPtr[TConnId]   = &DataPtr[TLS_HS_S_TLS_CERT_LEN_LEN + TLS_HS_S_TLS_CERT_LEN_LEN];
      Tls_CrlServerCertLen[TConnId]   = CurrentCertLen;
      Tls_CrlMoreThanOneCert[TConnId] = MoreThanOneCert;
      Tls_CrlIntermediateCrlChecked[TConnId] = FALSE;
      Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_CHOOSE_CRL;
#else
      /* trigger checking the certificate (chain) */
      Tls_CryptoCalcPending[TConnId] = TLS_CRYPTO_CALC_RX_SERVER_CERT_CHAIN;
#endif

      /* Leave the current function now, the check will be done in a lower prio context for not disturbing the normal operation of the ECU. */
      return LocReadPos;
    }
  }

  return LocReadPos;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  Tls_VHs_RxServerCert_ChainCheck
 **********************************************************************************************************************/
/*! \brief      Check the received server certificate chain
 *  \param[in]  TConnId        Id of the TLS connection
 *  \return     E_OK           validation trigger succeeded
 *  \return     E_NOT_OK       validation trigger failed
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_RxServerCert_ChainCheck(
    uint8 TConnId )
{

  uint32 AllCertsRemLen = Tls_CryptoCalcDataLen[TConnId];
  uint32 CurrentCertLen;
  uint8 *DataPtr = Tls_CryptoCalcDataPtr[TConnId];  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
  uint8 *LocDataPtr;
  uint32 LocReadPos = 0;

  if(AllCertsRemLen != 0)
  {
#if (TLS_SUPPORT_CRL == STD_ON)
    Std_ReturnType RetValue = E_OK;

    /* check CRL against intermediate CA */
    if( (Tls_CrlValidationState[TConnId] != TLS_CRL_VALIDATION_IDLE) &&
        (Tls_CrlIntermediateCrlChecked[TConnId] == FALSE) )
    {
      /* validate the signature of the used CRL using the key of the intermediate CA (received part of the cert chain) */
      uint8_least  CrlId         = Tls_CrlNextCrlIdToCheck[TConnId];
      uint8       *CrlPtr        = &Tls_CrlNvmExchData[0];
      uint8       *IntermediateCaCertPtr;
      uint16       IntermediateCaCertLen;
      uint8       *ClrSignAlgPtr = &CrlPtr[Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_CRL_SIGN_ALG_ID].ContOffs];  /* pointer to signature algorithm identifier in the CRL */
      uint8        ClrSignAlgLen = (uint8)Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_CRL_SIGN_ALG_ID].ContLen;
      uint16       CrlSignAlgorithm;

      Tls_Cert_Descr  CrlToCheckDesc;  /* pointer to a descriptor for the CRL that shall be validated (use the descriptor type of certificates) */
      Tls_Cert_Descr *CrlToCheckDescPtr = &CrlToCheckDesc;  /* pointer to a descriptor for the CRL that shall be validated (use the descriptor type of certificates) */
      Tls_Cert_Descr *CertValidDescPtr = Tls_RxCertDescChildPtr[TConnId];  /* pointer to a descriptor for the certificate of the intermediate CA */

      /* Find algorithm and key. The used signature algorithm can be read from the CRL */
      CrlSignAlgorithm = Tls_VHs_ReadSignAlgId(ClrSignAlgPtr, ClrSignAlgLen);

      IntermediateCaCertPtr = &DataPtr[LocReadPos + TLS_HS_S_TLS_CERT_LEN_LEN];  /* pointer to certificate of the intermediate CA */
      IntermediateCaCertLen = (uint16)(AllCertsRemLen - TLS_HS_S_TLS_CERT_LEN_LEN);

      CrlToCheckDescPtr->CertCh1.ContLen        = Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_CRL_CHAPTER_1].ContLen;
      CrlToCheckDescPtr->CertCh1.ContOffs       = Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_CRL_CHAPTER_1].ContOffs;
      CrlToCheckDescPtr->SignedCert.ContLen     = Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_TBS_CERT_LIST].ContLen;
      CrlToCheckDescPtr->SignedCert.ContOffs    = Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_TBS_CERT_LIST].ContOffs;
      CrlToCheckDescPtr->CertSignature.ContLen  = Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_CRL_SIGN].ContLen -1;
      CrlToCheckDescPtr->CertSignature.ContOffs = Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_CRL_SIGN].ContOffs +1;

      if(E_OK == Tls_VParseServerCert(CertValidDescPtr, TLS_HASH_INFO_NONE, IntermediateCaCertPtr, IntermediateCaCertLen))
      {
        /* veryfy (RSA or ECDSA based) signature of the received server certificate */
        switch(CrlSignAlgorithm)
        {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
#if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
        case TLS_HASH_SIGN_ALG_ID_MD5_RSA:
          {
            RetValue = Tls_VHs_VerifyCertSignMd5Rsa(TConnId, IntermediateCaCertPtr, CrlPtr, CertValidDescPtr, CrlToCheckDescPtr);
            break;
          }
#endif  /* (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON) */
        case TLS_HASH_SIGN_ALG_ID_SHA1_RSA:
          {
            RetValue = Tls_VHs_VerifyCertSignSha1Rsa(TConnId, IntermediateCaCertPtr, CrlPtr, CertValidDescPtr, CrlToCheckDescPtr);
            break;
          }
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
        case TLS_HASH_SIGN_ALG_ID_SHA1_ECDSA:
          {
            RetValue = Tls_VHs_VerifyCertSignXxEcdsa( TConnId, TLS_HASH_ALGORITHM_SHA1, IntermediateCaCertPtr, CrlPtr, CertValidDescPtr, CrlToCheckDescPtr);
            break;
          }
        case TLS_HASH_SIGN_ALG_ID_SHA256_ECDSA:
          {
            RetValue = Tls_VHs_VerifyCertSignXxEcdsa( TConnId, TLS_HASH_ALGORITHM_SHA256, IntermediateCaCertPtr, CrlPtr, CertValidDescPtr, CrlToCheckDescPtr);
            break;
          }
#endif
        default:
          {
            RetValue = E_NOT_OK;
          }
        }
      }

      Tls_CrlIntermediateCrlChecked[TConnId] = TRUE;
      if(RetValue != E_OK)
      {
        /* error, invalid CRL signature -> cancel handshake */
        return E_NOT_OK;
      }
    }
#endif

    {
      do
      {
        /* swap current and previous descriptor pointer */
        {
          Tls_Cert_Descr *CertDescrTempPtr;
          CertDescrTempPtr                  = Tls_RxCertDescChildPtr[TConnId];
          Tls_RxCertDescChildPtr[TConnId]   = Tls_RxCertDescCurrentPtr[TConnId];
          Tls_RxCertDescCurrentPtr[TConnId] = CertDescrTempPtr;
        }
        /* read length of current certificate */
        TLS_GET_UINT24(DataPtr, LocReadPos, CurrentCertLen);
        /* make the old current cert the new child cert, and make the cert in the new data the new current cert */
        LocDataPtr = &DataPtr[LocReadPos + TLS_HS_S_TLS_CERT_LEN_LEN];
        Tls_RxCertChildPtr[TConnId] = Tls_RxCertCurrentPtr[TConnId];
        Tls_RxCertCurrentPtr[TConnId] = LocDataPtr;

#if (TLS_SUPPORT_OCSP == STD_ON)
        Tls_Ocsp_CertChainLen[TConnId]++;  /* inc the counter of cert chain elements */
        TLS_LOG_HS_DIV_1("Tls_VHs_RxServerCert: cert chain length is now %d", Tls_Ocsp_CertChainLen[TConnId]);
#endif

        /* parse the cert */
        if(E_OK != Tls_VParseServerCert(Tls_RxCertDescCurrentPtr[TConnId], (TLS_HASH_INFO_ISSUER | TLS_HASH_INFO_SUBJECT), LocDataPtr, (uint16)CurrentCertLen))
        {
          /* parsing the certificate failed */
          return E_NOT_OK;
        }

        /* verify signature of previous certificate in the chain */
        if(E_OK != Tls_VHs_VerifyCertSign(TConnId,  Tls_RxCertCurrentPtr[TConnId], Tls_RxCertChildPtr[TConnId], Tls_RxCertDescCurrentPtr[TConnId], Tls_RxCertDescChildPtr[TConnId]))
        {
          /* error, cancel handshake */
          Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_CERT_UNKNOWN);
          return E_NOT_OK;
        }

#if (TLS_SUPPORT_OCSP == STD_ON)
        /* copy cert public key */
        if(E_OK != Tls_VCopyPubKeyFromCert(TConnId, LocDataPtr, Tls_RxCertDescCurrentPtr[TConnId]))
        {
          /* error, copy failed */
          TLS_LOG_HS_DIV_1("Tls_VHs_RxServerCert: error, copying cert %d failed", Tls_Ocsp_CertChainLen[TConnId]);
          return E_NOT_OK;
        }
#endif

        /* hash message */
        Tls_VHs_HashRxMsg(TConnId, &DataPtr[LocReadPos], (uint16)(CurrentCertLen + TLS_HS_S_TLS_CERT_LEN_LEN));

        /* go to next cert */
        AllCertsRemLen -= (uint32)(CurrentCertLen + TLS_HS_S_TLS_CERT_LEN_LEN);
        LocReadPos     += (uint32)(CurrentCertLen + TLS_HS_S_TLS_CERT_LEN_LEN);
      } while(AllCertsRemLen != 0);

      /* set pointer to the last received certificate */
      Tls_HsCurrDataPtr[TConnId]   = LocDataPtr;
      {
        uint32 NewReadPos = Tls_HsContinueReadPos[TConnId] + LocReadPos;
        if(NewReadPos == Tls_RxPlainTextFillLevel[TConnId])
        {
          Tls_HsContinueReadPos[TConnId] = 0;
        }
        else
        {
          Tls_HsContinueReadPos[TConnId] = (uint16)NewReadPos;
        }
      }
    }
  }
  else
  {
    /* there is no data to proceed */
  }

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (TLS_SUPPORT_CRL)
/***********************************************************************************************************************
 *  Tls_VHs_RxServerCert_FindMatchingCrl
 **********************************************************************************************************************/
/*! \brief      Find the matching CRL for the received server certificate
 *  \param[in]  TConnId        Id of the TLS connection
 *  \return     void
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_RxServerCert_FindMatchingCrl(
    Tls_TConnIdType TConnId )
{
  uint8_least      CrlIndex;
  Tls_Cert_Descr  *ServerCertDescrPtr = Tls_RxCertDescCurrentPtr[TConnId];

  /* find the matching CRL for the received server certificate */
  for(CrlIndex = 0; CrlIndex < TLS_CFG_NUM_CRLS; CrlIndex++)
  {
    /* compare crl issuer with cert issuer */
    if(ServerCertDescrPtr->Issuer.ContLen == Tls_CfgCrlElePosArrays[CrlIndex][TLS_CRL_ELE_ISSUER].ContLen)
    {
      uint8 *CertIssuerHasgPtr = &(ServerCertDescrPtr->Hash8Sha1Issuer[0]);
      uint8 *CrlIssuerHashPtr  = &(Tls_CrlDescs[CrlIndex].Hash8Sha1Issuer[0]);
      if(IPBASE_CMP_EQUAL == IpBase_StrCmpLen(CertIssuerHasgPtr, CrlIssuerHashPtr, TLS_CERT_INT_HASH_LEN))  /* compare hashes */
      {
        /* -> if the hash is equal, then probably also the real issuer string is equal */

        /* -> TRIGGER reading CRL from NVM */
        Tls_CrlValidationState[TConnId]    = TLS_CRL_VALIDATION_GET_CRL_FROM_NVM;
        Tls_CrlConnNvmRequBlockId[TConnId] = 0xFFFF;
        Tls_CrlNextCrlIdToCheck[TConnId]   = CrlIndex;  /* next crl that shall be read from NVM */
        break;
      }
    }
  }  /* for-loop */

  if(CrlIndex == TLS_CFG_NUM_CRLS)
  {
    /* no matching CRL found for the received server certificate */
    /* it is assumed that the cert is valid regarding the CRL check */
    Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_DONE;
  }
}

/***********************************************************************************************************************
 *  Tls_VCrlReadCrlFromNvm
 **********************************************************************************************************************/
/*! \brief      Read the CRL from the NVM to the NvmExchangeBlock.
 *  \description   This function is called from the Tls_MainFunction context.
 *  \param[in]  TConnId        Id of the TLS connection
 *  \return     void
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VCrlReadCrlFromNvm(
    Tls_TConnIdType TConnId )
{
  uint8_least    CrlIndex;
  Std_ReturnType Ret;
  uint8          NvMReadState;

  CrlIndex = Tls_CrlNextCrlIdToCheck[TConnId];

  switch(Tls_CrlConnNvmRequStatus[TConnId])
  {
  case TLS_NVM_REQU_IDLE:
    {
      /* start requesting the NVM block */
      if(Tls_CrlConnNvmRequBlockId[TConnId] == 0xFFFF)
      {
        Tls_CrlConnNvmRequBlockId[TConnId] = Tls_CfgCrlBlockIds[CrlIndex];
      }

      /* get the status of the NvM block */
      (void)NvM_GetErrorStatus(Tls_CfgCrlBlockIds[CrlIndex], &NvMReadState);  /* return value is always OK if input parameters are valid */
      /* check if the NvM block is not busy */
      if ( NVM_REQ_PENDING != NvMReadState )
      {
        /* try to read the NvM block */
        Ret = NvM_ReadBlock(Tls_CfgCrlBlockIds[CrlIndex], &Tls_CrlNvmExchData[0]);
        /* check if the NvM accepted the read request */
        if(Ret == E_OK)
        {
          /* Request is accepted. Poll request state in mainfunction. */
          Tls_CrlConnNvmRequStatus[TConnId] = TLS_NVM_REQU_PEND;
        }
        /* an error occurred, NvM could not read the NvM block */
        else
        {
          /* Request was not accepted, request has to be repeated in mainfunction. */
          Tls_CrlConnNvmRequStatus[TConnId] = TLS_NVM_REQU_BUSY;
        }
      }
      /* NvM block is busy */
      else
      {
        /* Request was not accepted, request has to be repeated in mainfunction. */
        Tls_CrlConnNvmRequStatus[TConnId] = TLS_NVM_REQU_BUSY;
      }
      break;
    }
  case TLS_NVM_REQU_BUSY:
    {
      /* retry requesting the NVM block */
      (void)NvM_GetErrorStatus(Tls_CfgCrlBlockIds[CrlIndex], &NvMReadState);  /* return value is always OK if input parameters are valid */
      /* check if the NvM block is not busy */
      if ( NVM_REQ_PENDING != NvMReadState )
      {
        /* try to read the NvM block */
        Ret = NvM_ReadBlock(Tls_CfgCrlBlockIds[CrlIndex], &Tls_CrlNvmExchData[0]);
        /* check if the NvM accepted the read request */
        if(Ret == E_OK)
        {
          /* Request is accepted. Poll request state in mainfunction. */
          Tls_CrlConnNvmRequStatus[TConnId] = TLS_NVM_REQU_PEND;
        }
        else
        {
          /* Request was not accepted, request has to be repeated in mainfunction. */
          /* Tls_CrlConnNvmRequStatus[TConnId] is already in state TLS_NVM_REQU_BUSY */
        }
      }
      break;
    }
  case TLS_NVM_REQU_PEND:
    {
      /* wait for the block to be read */
      (void)NvM_GetErrorStatus(Tls_CfgCrlBlockIds[CrlIndex], &NvMReadState);  /* return value is always OK if input parameters are valid */
      /* check if the NvM block is not busy */
      if( (NVM_REQ_OK == NvMReadState) || (NVM_REQ_RESTORED_FROM_ROM == NvMReadState) || (NVM_REQ_NV_INVALIDATED == NvMReadState) )
      {
        /* data is now available in the RAM block 'Tls_CrlNvmExchData' */
        /* switch to next state */
        Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_CHECK_UPDATE_TIME_AND_SN;
        Tls_CrlConnNvmRequStatus[TConnId] = TLS_NVM_REQU_IDLE;
      }
      break;
    }
  default:
    {
      /* nothing to do */
    }
  }
}

/***********************************************************************************************************************
 *  Tls_VCrlCheckCrlUpdateTime
 **********************************************************************************************************************/
/*! \brief         Check the update time of the CRL.
 *  \description   Check the values of the CRL's 'thisUpdate' and 'nextUpdate' compared to the current time. The
 *                 validation state is written to the variable Tls_CrlValidationState[].
 *  \param[in]     TConnId        Id of the TLS connection
 *  \return        void
 *  \context       task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VCrlCheckCrlUpdateTime(
    Tls_TConnIdType TConnId )
{
  uint8 *CrlPtr = &Tls_CrlNvmExchData[0];
#if (TLS_SUPPORT_REAL_TIME_CLOCK == STD_ON)
  uint32 Time32Now;
  uint32 TimeThisUpdate;
  uint32 TimeNextUpdate;
#endif
  uint8_least CrlCheckState;
  uint8_least CrlIndex = Tls_CrlNextCrlIdToCheck[TConnId];
  uint8  *CertListPtr;
  uint16  CertListLen;
  uint16 CertListExtraLen;
  boolean UpdateTimeIsValid;
  Tls_Cert_Descr *ServerCertDescrPtr = Tls_RxCertDescCurrentPtr[TConnId];

  CrlCheckState = 1;  /* matching CRL was found */

#if (TLS_SUPPORT_REAL_TIME_CLOCK == STD_ON)
  Tls_GetTime32Fct(&Time32Now);
  /* the same function to read the CRL times can be used as for reading the times in a certificate */
  Tls_VReadCertTimeInfo(&CrlPtr[Tls_CfgCrlElePosArrays[CrlIndex][TLS_CRL_ELE_THIS_UPDATE].ContOffs], (uint8)Tls_CfgCrlElePosArrays[CrlIndex][TLS_CRL_ELE_THIS_UPDATE].ContLen, &TimeThisUpdate);
  Tls_VReadCertTimeInfo(&CrlPtr[Tls_CfgCrlElePosArrays[CrlIndex][TLS_CRL_ELE_NEXT_UPDATE].ContOffs], (uint8)Tls_CfgCrlElePosArrays[CrlIndex][TLS_CRL_ELE_NEXT_UPDATE].ContLen, &TimeNextUpdate);

  /* check CRL times */
  if(TimeThisUpdate < Time32Now)
#endif
  {
    /* CRL time 'thisUpdate' is in a valid state */
    const uint8 *CertSerialNumPtr;
    uint8        CertSerialNumLen;
    uint8        CrlResult;

    UpdateTimeIsValid = TRUE;

#if (TLS_SUPPORT_REAL_TIME_CLOCK == STD_ON)
    if(TimeNextUpdate >= Time32Now)
    {
      CrlCheckState = 3;  /* CRL is valid and in its intendet live time state */
    }
    else
    {
      /* 'error', CRL should already have been update. This does not mean that the list is not valid any more, it still can be used but maybe a newer list would contain more invalidated entries. */
      CrlCheckState = 4;
    }
#endif

    /* search inside the CRL for the certs serial number */
    CertSerialNumPtr = &(Tls_CrlServerCertPtr[TConnId])[ServerCertDescrPtr->SerialNumber.ContOffs];
    CertSerialNumLen = (uint8)ServerCertDescrPtr->SerialNumber.ContLen;

    CertListLen = Tls_CfgCrlElePosArrays[CrlIndex][TLS_CRL_ELE_REVOKED_CERTS].ContLen;
    if(CertListLen < 0x80)
    {
      CertListExtraLen = 2;  /* one byte type, one byte length field */
    }
    else if(CertListLen < 0x8000)
    {
      CertListExtraLen = 3;  /* one byte type, two byte length field */
    }
    else
    {
      CertListExtraLen = 4;  /* one byte type, three byte length field */
    }
    CertListLen += CertListExtraLen;
    CertListPtr = &CrlPtr[Tls_CfgCrlElePosArrays[CrlIndex][TLS_CRL_ELE_REVOKED_CERTS].ContOffs - CertListExtraLen];

    if( (E_OK == Tls_VSearchForSerialInCrl(CertListPtr, CertListLen, CertSerialNumPtr, CertSerialNumLen, &CrlResult)) &&
        (CrlResult == TLS_CRL_SEARCH_STATUS_CERT_NOT_REVOKED) )
    {
      /* parsing the cert list was successful */
      /* the cert was not listed / found in the CRL, so probably the cert is still valid */

      /* switch to next state */
      Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_DONE;
    }
    else
    {
      /* The cert is listed in the CRL, or there was a problem parsing the CRL */
      /* cancel handshake */
      CrlCheckState = 5;
      Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_FAILED;
    }

  }
#if (TLS_SUPPORT_REAL_TIME_CLOCK == STD_ON)
  else
  {
    /* 'error', CRL is not yet valid */
    CrlCheckState = 2;  /* matching CRL was found but is not yet valid */
    Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_FAILED;
  }
#endif
}

/***********************************************************************************************************************
 *  Tls_VSearchForSerialInCrl
 **********************************************************************************************************************/
/*! \brief      Search for a given certificate serial number in a CRL.
 *  \description   The underlying function IpBase_BerGetElement can only handle chapters with index samller than 255, so
 *                 this is a limitation for the number of certs in the cert list of a CRL that can be handled by this
*                  implementation.
 *  \param[in]  CertListPtr    Pointer to the certificate list inside the CRL
 *  \param[in]  CertListLen    Length of the certificate list inside the CRL
 *  \param[in]  SerialPtr      Pointer to the serial number that shall be found in the CRL
 *  \param[in]  SerialLen      Length of the serial number
 *  \param[out] CrlSeachStatusPtr   Pointer to the status of the search
 *  \return     E_OK           Serial number was not found inside the CRL
 *  \return     E_NOT_OK       Serial number was found inside the CRL
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VSearchForSerialInCrl(
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertListPtr,
    uint16 CertListLen,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SerialPtr,
    uint8 SerialLen,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) CrlSeachStatusPtr )
{
  Std_ReturnType              RetVal              = E_NOT_OK;  /* default value means that the cert is part of the cert list or an error occured */
  CONST(uint8, TLS_CONST)     EleNrCertRoot[3]    = { 1, 0,0 };  /* chapter 1: root of the list */
  uint8                       EleNrSerial[3];
  CONST(uint8, TLS_CONST)     MaxEleDepth         = 3;
  IpBase_BerWorkspaceType     Workspace;
  IpBase_BerStackElementType  BerStack[3];  /* max depth of one element */
  IpBase_BerElementType       EleDesc;
  uint32                      RemainingDataLen;
  uint8_least                 CertCounter;

  IpBase_BerInitWorkspace(&Workspace, &BerStack[0], MaxEleDepth); /* inti workspace, only needed once */

  /* Check that the given length of the cert list matches the length value at the beginning of the cert list. */
  if( IPBASE_E_OK == IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrCertRoot[0], 1, CertListPtr, CertListLen) )
  {
    /* now, EleDesc contains the index information of chapter one */
    if(CertListLen == (EleDesc.EndIdx - EleDesc.Idx))
    {
      RetVal = E_OK;  /* cert list length is valid */ /* CrlSeachStatusPtr is set later */
    }
    else
    {
      /* error, the given length of the cert list does not match the length value at the beginning of the cert list */
      *CrlSeachStatusPtr = TLS_CRL_SEARCH_STATUS_INT_ERROR;
    }
  }
  else
  {
    *CrlSeachStatusPtr = TLS_CRL_SEARCH_STATUS_INT_ERROR;
  }

  if(RetVal == E_OK)
  {
    EleNrSerial[0] = 1;
    EleNrSerial[2] = 1;
    RemainingDataLen = EleDesc.EndIdx - EleDesc.ContentIdx;  /* remove type and length fields */

    *CrlSeachStatusPtr = TLS_CRL_SEARCH_STATUS_CERT_NOT_REVOKED;

    /* limit of our implementation: 254 (revoked) certificates in the CRL */
    for(CertCounter = 1; ((RemainingDataLen != 0) && (CertCounter < 255)); CertCounter++)
    {
      uint32 CertEntryLen;

      EleNrSerial[1] = (uint8)CertCounter;  /* we are always looking for the chapters '1.x.1' */

      /* get cert entry length */
      if( IPBASE_E_OK == IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrSerial[0], 2, CertListPtr, CertListLen) )
      {
        CertEntryLen = EleDesc.EndIdx - EleDesc.Idx;
      }

      if( IPBASE_E_OK == IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrSerial[0], 3, CertListPtr, CertListLen) )
      {
        /* now, EleDesc contains the index information */
        uint32 LocSerialLen = EleDesc.EndIdx - EleDesc.ContentIdx;
        const uint8 *LocSerialPtr  = &CertListPtr[EleDesc.ContentIdx];
        if( (LocSerialLen == SerialLen) /* the length of the serial number is valid, so now check the full serial number */ &&
            (IPBASE_CMP_EQUAL == IpBase_StrCmpLen(LocSerialPtr, SerialPtr, SerialLen)) /* matching serial number found */ )
        {
          /* The cert is listed in the CRL */

          /* Checking the time is not necessary, since the CRL only contains invalidated certificates. The
             invalidation reason is not of interest for us.
             -> cert is revoked, so cancel the TLS handshake */
          *CrlSeachStatusPtr = TLS_CRL_SEARCH_STATUS_CERT_REVOKED;
          RetVal = E_NOT_OK;
          break;  /* break the for-loop */
        }
        RemainingDataLen -= CertEntryLen;
      }
      else
      {
        /* parse error */
        *CrlSeachStatusPtr = TLS_CRL_SEARCH_STATUS_INT_ERROR;
        RetVal = E_NOT_OK;
        break;  /* break the for-loop */
      }
    }  /* for-loop */
    if((RemainingDataLen != 0) && (RetVal == E_OK))
    {
      /* The cert was not found in the first 254 certs in the CRL but there are more certs in the list */
      *CrlSeachStatusPtr = TLS_CRL_SEARCH_STATUS_TOO_MANY_CERTS;
      RetVal = E_NOT_OK;
      /* for security reasons it is assumed that the cert is listed in the CRL, somewhere in the remaining (uncecked) certs in the CRL */
      TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_INTERNAL_FUNCTION, TLS_EVENT_LOOKING_FOR_CERT_IN_CRL, TLS_EVENT_STATUS_FAILED)  /* report event */
    }
  }

  return RetVal;
}
#endif /* (TLS_SUPPORT_CRL) */

#if (TLS_SUPPORT_REAL_TIME_CLOCK == STD_ON)
/***********************************************************************************************************************
 *  Tls_VHs_CheckCertTimeValidity
 **********************************************************************************************************************/
/*! \brief      check validity (time) of a certificate
 *  \param[in]  CertStartPtr   pointer to the start of the certificate
 *  \param[in]  CertDescrPtr   pointer to the certificate descriptor
 *  \return     E_OK           certificate validity time is ok
 *  \return     E_NOT_OK       certificate validity time is not ok
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_CheckCertTimeValidity(
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertStartPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertDescrPtr )
{
  /* check validity (time) of received certificate */
  uint32 TimeNotBefore;
  uint32 TimeNotAfter;
  uint32 Time32Now;

  /* get current (system) time */
  Tls_GetTime32Fct(&Time32Now);
  Tls_VReadCertTimeInfo( &(CertStartPtr[CertDescrPtr->ValidNotBefore.ContOffs]), (uint8)(CertDescrPtr->ValidNotBefore.ContLen), &TimeNotBefore );
  Tls_VReadCertTimeInfo( &(CertStartPtr[CertDescrPtr->ValidNotAfter.ContOffs]),  (uint8)(CertDescrPtr->ValidNotAfter.ContLen),  &TimeNotAfter );

  /* compare with validity start and end */
  if( (Time32Now <= TimeNotAfter) &&
    (Time32Now > TimeNotBefore) )
  {
    /* OK */
    TLS_LOG_HS_DIV("Tls_VHs_CheckCertTimeValidity: server_certificate validity time is ok");
    return E_OK;
  }
  else
  {
    /* NOT OK */
    /* cert is outdated */
    TLS_LOG_HS_DIV("Tls_VHs_CheckCertTimeValidity: error: server_certificate validity time is NOT ok");

    /* certificate is out of date */
    return E_NOT_OK;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif
  /* TLS_SUPPORT_REAL_TIME_CLOCK */

/***********************************************************************************************************************
 *  Tls_VHs_CheckRecServCert
 **********************************************************************************************************************/
/*! \brief      Check received server cert to match one of the stored server certs (known auth instances)
 *  \param[in]  TConnId        TLS connection id
 *  \return     E_OK           no precessing error
 *  \return     E_NOT_OK       precessing error or no matching server cert
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_CheckRecServCert(
    uint8 TConnId )
{
  uint8 ServerCertIdx;
  Tls_Cert_Descr *CertDescrPtr = Tls_RxCertDescCurrentPtr[TConnId];

  if(TLS_NVM_REQU_READY != Tls_ConnNvmRequStatus[TConnId])
  {
    /* go through all stored server certs to find the corresponding one (compare first 8 bytes of hash) */
    {
      for(; 0 != Tls_NumRemServCerts[TConnId]; Tls_NumRemServCerts[TConnId]--)
      {
        ServerCertIdx = Tls_NumRemServCerts[TConnId] -1;
        if( (Tls_ServerCertDescs[ServerCertIdx].Subject.ContLen == CertDescrPtr->Issuer.ContLen) &&  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
          (IPBASE_CMP_EQUAL == IpBase_StrCmpLen(&Tls_ServerCertDescs[ServerCertIdx].Hash8Sha1Subject[0], &CertDescrPtr->Hash8Sha1Issuer[0], TLS_CERT_INT_HASH_LEN)) )
        {
          /* request cert from NVM */
          if(TLS_SERV_CERT_PARSE_COMPLETE == Tls_ServCertsParseStatus)
          {
            if(TLS_NVM_REQU_IDLE == Tls_ConnNvmRequStatus[TConnId])
            {
              Std_ReturnType Ret;
              uint8          NvMReadState;
              Tls_ConnNvmRequBlockId[TConnId] = Tls_VCfgGetServerCertBlockIds(ServerCertIdx);
              Tls_CurrentServCertId[TConnId] = ServerCertIdx;

              /* get the status of the NvM block */
              (void)NvM_GetErrorStatus(Tls_ConnNvmRequBlockId[TConnId], &NvMReadState);  /* return value is always OK if input parameters are valid */
              /* check if the NvM block is not busy */
              if ( NVM_REQ_PENDING != NvMReadState )
              {
                /* try to read the NvM block */
                Ret = NvM_ReadBlock(Tls_ConnNvmRequBlockId[TConnId], &Tls_NvmExchData[0]);
                /* check if the NvM accepted the read request */
                if(Ret == E_OK)
                {
                  /* Request is accepted. Poll request state in mainfunction. */
                  Tls_ConnNvmRequStatus[TConnId] = TLS_NVM_REQU_PEND;
                }
                /* an error occurred, NvM could not read the NvM block */
                else
                {
                  /* Request was not accepted, request has to be repeated in mainfunction. */
                  Tls_ConnNvmRequStatus[TConnId] = TLS_NVM_REQU_BUSY;
                }
              }
              /* NvM block is busy */
              else
              {
                /* Request was not accepted, request has to be repeated in mainfunction. */
                Tls_ConnNvmRequStatus[TConnId] = TLS_NVM_REQU_BUSY;
              }
              return E_OK;
            }
            else
            {
              /* TLS internal error, this should never happen */
              /* cancel handshake */
              return E_NOT_OK;
            }
          }

          /* valid server (root) cert found */
          TLS_LOG_HS_DIV("Tls_VHs_CheckRecServCert: issuer check: matching server (root) cert found");
          break;
        }
        else
        {
          TLS_LOG_HS_DIV("Tls_VHs_CheckRecServCert: issuer check: currently checked subject is not the same as the received issuer");
        }
      }
      if(0 == Tls_NumRemServCerts[TConnId])
      {
        /* no corresponing parent cert was found -> cancel handshake */
        TLS_LOG_HS_DIV("Tls_VHs_CheckRecServCert: ERROR: no valid server (root) cert found -> cancel TLS handshake");
        return E_NOT_OK;
      }
    }

  }
  else
  {
    /* TLS_NVM_REQU_READY == Tls_ConnNvmRequStatus[TConnId] */

    /* check validity period */
#if (TLS_SUPPORT_REAL_TIME_CLOCK == STD_ON)
    if(E_OK != Tls_VHs_CheckCertTimeValidity(&Tls_NvmExchData[0], &Tls_ServerCertDescs[Tls_CurrentServCertId[TConnId]]))
    {
      /* The matching stored root cert is currently not valid (invalid validity period) */
      TLS_REPORT_ERROR_STATUS(TLS_API_ID_V_INTERNAL_FUNCTION, TLS_EVENT_ROOT_CERT_VALIDITY_TIME_CHECK, TLS_EVENT_STATUS_FAILED)  /* report event */
      /* error, cancel handshake */
      return E_NOT_OK;
    }
#endif

    /* set crypto-flag */
    Tls_CryptoCalcPending[TConnId] = TLS_CRYPTO_CALC_RX_SERVER_CERT_LAST;
    /* the signature validation will be done in the low prio task */

  }

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VHs_CheckLastCertAgainstRoot
 **********************************************************************************************************************/
/*! \brief      Check last received cert to match one of the stored server certs (known auth instances). <br>
 *  \description   Check either the last certificate of the received cert chain, or the received server certificate if
 *                 no cert chain was received.
 *  \param[in]  TConnId        TLS connection id
 *  \return     void
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_CheckLastCertAgainstRoot(
    uint8 TConnId )
{
  P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) rxCertPtr         = Tls_HsCurrDataPtr[TConnId];
  Std_ReturnType                         retVal;
  uint8                                  validationResult;

  /* verify server cert signature */
  retVal = Tls_VHs_VerifyCertSign(TConnId,  &Tls_NvmExchData[0], rxCertPtr, &Tls_ServerCertDescs[Tls_CurrentServCertId[TConnId]], Tls_RxCertDescCurrentPtr[TConnId]);
  if(retVal == E_OK)
  {
    validationResult = TCPIP_TLS_VALIDATION_OK;
  }
  else
  {
    validationResult = TCPIP_TLS_VALIDATION_LAST_SIGN_INVALID;
  }
  TcpIp_Tcp_TlsValidationResult((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[TConnId], &validationResult, Tls_HsServerCertChainPtr[TConnId], Tls_HsServerCertChainLen[TConnId]);

  if(validationResult != TCPIP_TLS_VALIDATION_OK)
  {
    /* error, verification failed or the socket owner invalidated the result -> cancel handshake */

    Tls_Rl_CurrentPart[TConnId]    = TLS_RL_IN_HDR;
    Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_BAD_CERT);
#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
    /* special SCC error indication */
    Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[TConnId], TLS_E_UNKNOWN_CA, TRUE);  /* no matching root cert or validation failed */
#endif
    Tls_CryptoCalcPending[TConnId] = TLS_CRYPTO_CALC_IDLE;  /* reset the crypto-flag */
    return;
  }

#if (TLS_SUPPORT_GET_NVM_BLOCK_ID_FOR_USED_ROOT_CERT == STD_ON)
  if(retVal == E_OK)
  {
    /* signature could be validated, so store the NVM Block ID of the used root certificate */
    Tls_UsedRootCertNvmBlockId[TConnId] = Tls_VCfgGetServerCertBlockIds(Tls_CurrentServCertId[TConnId]);
  }
  else
  {
    /* Since none of the stored root certificates was used to validate the received server certificate chain, the
       variable Tls_UsedRootCertNvmBlockId[] stays uninitialized for this connection. This fact is known to the socket
       owner since the socket owner has been informed about the unknown CA. */
  }
#endif

  /* continue */
  Tls_ConnNvmRequStatus[TConnId] = TLS_NVM_REQU_IDLE;
  Tls_HandShakeState[TConnId]    = TLS_HS_STATE_CERT_REC;
  Tls_CryptoCalcPending[TConnId] = TLS_CRYPTO_CALC_IDLE;

  if(0 == Tls_HsContinueReadPos[TConnId])
  {
    /* continue with next record layer element */
    Tls_RxBlockingState[TConnId] = TLS_RX_BLOCKING_STATE_NONE;

  }
  else
  {
    /* continue with next handshake element inside record layer message */
    uint8 *PlainTextDataPtr;

    /* received server cert is validated successfully. Now call function to procede receiving handshake messages */
    PlainTextDataPtr = Tls_GetRxPlainTextStartPtr(TConnId);
    if(0 == Tls_VHs_Rx(TConnId, PlainTextDataPtr, Tls_Rl_DecryptedDataLen[TConnId], Tls_HsContinueReadPos[TConnId]))
    {
      return;
    }
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VHs_ReadSignAlgId
 **********************************************************************************************************************/
/*! \brief      read signature identifier
 *  \param[in]  SignAlgPtr     pointer to signature identifier
 *  \param[in]  SignAlgLen     length of signature identifier
 *  \return                    signature id (internal define)
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_ReadSignAlgId(
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SignAlgPtr,
    uint8 SignAlgLen )
{
  /* ASN1 certificate signature algorithms: */
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
# if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
  CONST(uint8, TLS_CONST) Tls_SignAlgMd5RsaV1[11]     = { 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x04 }; /* 1.2.840.113549.1.1.4 md5WithRSASignature */
# endif
  CONST(uint8, TLS_CONST) Tls_SignAlgSha1RsaV1[11]    = { 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x05 }; /* 1.2.840.113549.1.1.5 sha1WithRSASignature */
  CONST(uint8, TLS_CONST) Tls_SignAlgSha256RsaV1[11]  = { 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B }; /* 1.2.840.113549.1.1.11 sha256WithRSASignature */
# if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
  CONST(uint8, TLS_CONST) Tls_SignAlgMd5RsaV2[7]      = { 0x06, 0x05, 0x2B, 0x0E, 0x03, 0x02, 0x19 }; /* 1.3.14.3.2.25 md5WithRSASignature */
# endif
  CONST(uint8, TLS_CONST) Tls_SignAlgSha1RsaV2[7]     = { 0x06, 0x05, 0x2B, 0x0E, 0x03, 0x02, 0x1D }; /* 1.3.14.3.2.29 sha1WithRSASignature */
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
  CONST(uint8, TLS_CONST) Tls_SignAlgSha1EccV1[9]     = { 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x01 }; /* 1.2.840.10045.4.1 ecdsaWithSha1Signature */
  CONST(uint8, TLS_CONST) Tls_SignAlgSha256EccV1[10]  = { 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02 }; /* 1.2.840.10045.4.3.2 ecdsaWithSha256Signature */
#endif

  uint16                  SignAlgId;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */

  if(SignAlgLen < (SignAlgPtr[1] + 2))
  {
    /* error, length is invalid */
    return (uint16)TLS_HASH_SIGN_ALG_ID_INVALID;
  }

  if(0x09 == SignAlgPtr[1])
  {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
# if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
    /* compare bytes 0..9 */
    if(IPBASE_CMP_EQUAL != IpBase_StrCmpLen(&Tls_SignAlgMd5RsaV1[0], SignAlgPtr, 10))
    {
      /* error */
      SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;
    }
    else
# endif
    {
      /* read last byte */
      if(Tls_SignAlgSha1RsaV1[10] == SignAlgPtr[10])
      {
        SignAlgId = TLS_HASH_SIGN_ALG_ID_SHA1_RSA;  /* SHA1 */
      }
      else if(Tls_SignAlgSha256RsaV1[10] == SignAlgPtr[10])
      {
        SignAlgId = TLS_HASH_SIGN_ALG_ID_SHA256_RSA; /* SHA256 */
      }
# if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
      else if(Tls_SignAlgMd5RsaV1[10] == SignAlgPtr[10])
      {
        SignAlgId = TLS_HASH_SIGN_ALG_ID_MD5_RSA;  /* MD5 */
      }
# endif
      else
      {
        SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;  /* invalid */
      }
    }
#else
    SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;  /* invalid */
#endif
  }
  else if(0x05 == SignAlgPtr[1])
  {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
# if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
    /* compare bytes 0..5 */
    if(IPBASE_CMP_EQUAL != IpBase_StrCmpLen(&Tls_SignAlgMd5RsaV2[0], SignAlgPtr, 6))
    {
      /* error */
      SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;
    }
    else
# endif
    {
      /* read last byte */
      if(Tls_SignAlgSha1RsaV2[6] == SignAlgPtr[6])
      {
        SignAlgId = TLS_HASH_SIGN_ALG_ID_SHA1_RSA;  /* SHA1 */
      }
      else
      {
        SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;  /* invalid */
      }
# if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
      else if(Tls_SignAlgMd5RsaV2[6] == SignAlgPtr[6])
      {
        SignAlgId = TLS_HASH_SIGN_ALG_ID_MD5_RSA;  /* MD5 */
      }
# endif
    }
#else
    SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;  /* invalid */
#endif
  }
  else if(0x07 == SignAlgPtr[1])
  {
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
    /* compare bytes 0..6 */
    if(IPBASE_CMP_EQUAL != IpBase_StrCmpLen(&Tls_SignAlgSha1EccV1[0], SignAlgPtr, 7))
    {
      /* error */
      SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;
    }
    else
    {
      SignAlgId = TLS_HASH_SIGN_ALG_ID_SHA1_ECDSA;  /* ECDSA-SHA1 */
    }
#else
    SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;  /* invalid */
#endif
  }
  else if(0x08 == SignAlgPtr[1])
  {
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
    /* compare bytes 0..7 */
    if(IPBASE_CMP_EQUAL != IpBase_StrCmpLen(&Tls_SignAlgSha256EccV1[0], SignAlgPtr, 8))
    {
      /* error */
      SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;
    }
    else
    {
      SignAlgId = TLS_HASH_SIGN_ALG_ID_SHA256_ECDSA;  /* ECDSA-SHA256 */
    }
#else
    SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;  /* invalid */
#endif
  }
  else
  {
    /* not supported -> cancel handshake */
    SignAlgId = TLS_HASH_SIGN_ALG_ID_INVALID;  /* invalid */
  }

  return SignAlgId;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VHs_VerifyCertSign
 **********************************************************************************************************************/
/*! \brief      verifiy cert signature with any signature and hash algorithm
 *  \param[in]  TConnId              TLS connection id
 *  \param[in]  CertValidPtr         pointer to the (assumed) valid certificate
 *  \param[in]  CertToCheckPtr       pointer to the certificate that shall be verified
 *  \param[in]  CertValidDescrPtr    pointer to the descriptor of the valid certificate
 *  \param[in]  CertToCheckDescrPtr  pointer to the descriptor of the certificate that shall be verified
 *  \return     E_OK                 certificate could be verified
 *  \return     E_NOT_OK             certificate could NOT be verified
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSign(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr )
{
  Std_ReturnType RetValue;
  uint16         SignAlgId;  /* combination of signature and hash algorithm */  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
  const uint8*   SignAlgPtr = &(CertToCheckPtr[CertToCheckDescPtr->CertSignAlgId.ContOffs]);
  uint8          SignAlgLen = (uint8)(CertToCheckDescPtr->CertSignAlgId.ContLen);

  /* read signature algorithm identifier */
  SignAlgId  = Tls_VHs_ReadSignAlgId(SignAlgPtr, SignAlgLen);

  /* veryfy (RSA or ECDSA based) signature of the received server certificate */
  switch(SignAlgId)
  {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
#if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
  case TLS_HASH_SIGN_ALG_ID_MD5_RSA:
    {
      RetValue = Tls_VHs_VerifyCertSignMd5Rsa(TConnId, CertValidPtr, CertToCheckPtr, CertValidDescPtr, CertToCheckDescPtr);
      break;
    }
#endif  /* (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON) */
  case TLS_HASH_SIGN_ALG_ID_SHA1_RSA:
    {
      RetValue = Tls_VHs_VerifyCertSignSha1Rsa(TConnId, CertValidPtr, CertToCheckPtr, CertValidDescPtr, CertToCheckDescPtr);
      break;
    }
  case TLS_HASH_SIGN_ALG_ID_SHA256_RSA:
    {
      RetValue = Tls_VHs_VerifyCertSignXxRsa(TConnId, TLS_HASH_ALGORITHM_SHA256, CertValidPtr, CertToCheckPtr, CertValidDescPtr, CertToCheckDescPtr);
      break;
    }
#endif
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
  case TLS_HASH_SIGN_ALG_ID_SHA1_ECDSA:
    {
      RetValue = Tls_VHs_VerifyCertSignXxEcdsa( TConnId, TLS_HASH_ALGORITHM_SHA1, CertValidPtr, CertToCheckPtr, CertValidDescPtr, CertToCheckDescPtr);
      break;
    }
  case TLS_HASH_SIGN_ALG_ID_SHA256_ECDSA:
    {
      RetValue = Tls_VHs_VerifyCertSignXxEcdsa( TConnId, TLS_HASH_ALGORITHM_SHA256, CertValidPtr, CertToCheckPtr, CertValidDescPtr, CertToCheckDescPtr);
      break;
    }
#endif
  default:
    {
      RetValue = E_NOT_OK;
    }
  }

  return RetValue;
}

#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
#if (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON)
/***********************************************************************************************************************
 *  Tls_VHs_VerifyCertSignMd5Rsa
 **********************************************************************************************************************/
/*! \brief      verifiy cert signature (md5 with rsa)
 *  \param[in]  TConnId              TLS connection id
 *  \param[in]  CertValidPtr         pointer to the (assumed) valid certificate
 *  \param[in]  CertToCheckPtr       pointer to the certificate that shall be verified
 *  \param[in]  CertValidDescrPtr    pointer to the descriptor of the valid certificate
 *  \param[in]  CertToCheckDescrPtr  pointer to the descriptor of the certificate that shall be verified
 *  \return     E_OK                 certificate could be verified
 *  \return     E_NOT_OK             certificate could NOT be verified
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSignMd5Rsa(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr )
{
  eslt_WorkSpaceRSAMD5ver *LocWsRsaVerPtr = (eslt_WorkSpaceRSAMD5ver *)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
  eslt_ErrorCode           CvRetValue;
  const uint8  *ModulusPtr;
  const uint8  *ExponentPtr;
  uint16  ModulusLen;
  uint16  ExponentLen;
  const uint8  *ContPtr;
  uint16  ContLen;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */

  ModulusLen  = (uint16)CertValidDescPtr->KeyMain.ContLen;
  ExponentLen = (uint16)CertValidDescPtr->KeyExp.ContLen;
  ModulusPtr  = &CertValidPtr[CertValidDescPtr->KeyMain.ContOffs];
  ExponentPtr = &CertValidPtr[CertValidDescPtr->KeyExp.ContOffs];

  ContPtr = &CertToCheckPtr[CertToCheckDescPtr->CertCh1.ContOffs];  /* signed certificate (ch1.1) including type and lenth field */
  ContLen = (uint16)(CertToCheckDescPtr->SignedCert.ContLen + (CertToCheckDescPtr->SignedCert.ContOffs - CertToCheckDescPtr->CertCh1.ContOffs));

  CvRetValue = esl_initWorkSpaceHeader(&LocWsRsaVerPtr->header, ESL_MAXSIZEOF_WS_RSA_MD5_VER, NULL_PTR);  /* PRQA S 3393 */ /* MD_TLS_12.1_3393_3395 */
  if( ESL_ERC_NO_ERROR != CvRetValue)
  {
    /* error */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignMd5Rsa: sign verify: ws header init failed (in cert check)");
    return E_NOT_OK;
  }
  CvRetValue = Crypto_initVerifyRSAMD5_V15( LocWsRsaVerPtr, ModulusLen, ModulusPtr, ExponentLen, ExponentPtr );
  if( ESL_ERC_NO_ERROR != CvRetValue)
  {
    /* error */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignMd5Rsa: sign verify: init verify failed (in cert check)");
    return E_NOT_OK;
  }
  /* hash the complete content of the chapter 1.1 in the cert */
  CvRetValue = Crypto_updateVerifyRSAMD5_V15( LocWsRsaVerPtr, ContLen, ContPtr );
  if( ESL_ERC_NO_ERROR != CvRetValue)
  {
    /* error */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignMd5Rsa: sign verify: update verify failed (in cert check)");
    return E_NOT_OK;
  }

  CvRetValue = Crypto_finalizeVerifyRSAMD5_V15( LocWsRsaVerPtr, (uint16)(CertToCheckDescPtr->CertSignature.ContLen), &CertToCheckPtr[CertToCheckDescPtr->CertSignature.ContOffs] );
  if( ESL_ERC_NO_ERROR != CvRetValue)
  {
    /* error while processing the hash */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignMd5Rsa: sign verify: finalize verify failed (in cert check)");
    return E_NOT_OK;
  }
  else
  {
    /* success */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignMd5Rsa: hash compare succeeded, signature is valid (in cert check)");
  }
  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (TLS_ENABLE_SUPPORT_RSA_MD5_CERTS == STD_ON) */

/***********************************************************************************************************************
 *  Tls_VHs_VerifyCertSignSha1Rsa
 **********************************************************************************************************************/
/*! \brief      verifiy cert signature (sha1 with rsa)
 *  \param[in]  TConnId              TLS connection id
 *  \param[in]  CertValidPtr         pointer to the (assumed) valid certificate
 *  \param[in]  CertToCheckPtr       pointer to the certificate that shall be verified
 *  \param[in]  CertValidDescrPtr    pointer to the descriptor of the valid certificate
 *  \param[in]  CertToCheckDescrPtr  pointer to the descriptor of the certificate that shall be verified
 *  \return     E_OK                 certificate could be verified
 *  \return     E_NOT_OK             certificate could NOT be verified
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSignSha1Rsa(
    uint8 TConnId,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr )
{
  eslt_WorkSpaceRSAver *LocWsRsaVerPtr = (eslt_WorkSpaceRSAver *)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
  eslt_ErrorCode        CvRetValue;
  const uint8  *ModulusPtr;
  const uint8  *ExponentPtr;
  uint16  ModulusLen;
  uint16  ExponentLen;
  const uint8  *ContPtr;
  uint16  ContLen;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */

  ModulusLen  = (uint16)CertValidDescPtr->KeyMain.ContLen;
  ExponentLen = (uint16)CertValidDescPtr->KeyExp.ContLen;
  ModulusPtr  = &CertValidPtr[CertValidDescPtr->KeyMain.ContOffs];
  ExponentPtr = &CertValidPtr[CertValidDescPtr->KeyExp.ContOffs];

  ContPtr = &CertToCheckPtr[CertToCheckDescPtr->CertCh1.ContOffs];  /* signed certificate (ch1.1) including type and lenth field */
  ContLen = (uint16)(CertToCheckDescPtr->SignedCert.ContLen + (CertToCheckDescPtr->SignedCert.ContOffs - CertToCheckDescPtr->CertCh1.ContOffs));

  CvRetValue = esl_initWorkSpaceHeader(&LocWsRsaVerPtr->header, ESL_MAXSIZEOF_WS_RSA_VER, NULL_PTR);  /* SHA1 */  /* PRQA S 3393,3395 */ /* MD_TLS_12.1_3393_3395 */
  if( ESL_ERC_NO_ERROR != CvRetValue)
  {
    /* error */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignSha1Rsa: sign verify: ws header init failed (in cert check)");
    return E_NOT_OK;
  }
  CvRetValue = esl_initVerifyRSASHA1_V15( LocWsRsaVerPtr, ModulusLen, ModulusPtr, ExponentLen, ExponentPtr );
  if( ESL_ERC_NO_ERROR != CvRetValue)
  {
    /* error */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignSha1Rsa: sign verify: init verify failed (in cert check)");
    return E_NOT_OK;
  }
  /* hash the complete content of the chapter 1.1 in the cert */
  CvRetValue = esl_updateVerifyRSASHA1_V15( LocWsRsaVerPtr, ContLen, ContPtr );
  if( ESL_ERC_NO_ERROR != CvRetValue)
  {
    /* error */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignSha1Rsa: sign verify: update verify failed (in cert check)");
    return E_NOT_OK;
  }

  CvRetValue = esl_finalizeVerifyRSASHA1_V15( LocWsRsaVerPtr, (uint16)(CertToCheckDescPtr->CertSignature.ContLen), &CertToCheckPtr[CertToCheckDescPtr->CertSignature.ContOffs] );
  if( ESL_ERC_NO_ERROR != CvRetValue)
  {
    /* error while processing the hash */
    TLS_LOG_HS_CRYPT("Tls_VHs_VerifyCertSignSha1Rsa: sign verify: finalize verify failed (in cert check)");
    return E_NOT_OK;
  }
  else
  {
    /* successfully verified the signature */
  }
  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VHs_VerifyCertSignXxRsa
 **********************************************************************************************************************/
/*! \brief      verifiy cert signature (some hash with rsa)
 *  \param[in]  TConnId              TLS connection id
 *  \param[in]  HashAlg              Hash algorithm used for the cert signature
 *  \param[in]  CertValidPtr         pointer to the (assumed) valid certificate
 *  \param[in]  CertToCheckPtr       pointer to the certificate that shall be verified
 *  \param[in]  CertValidDescrPtr    pointer to the descriptor of the valid certificate
 *  \param[in]  CertToCheckDescrPtr  pointer to the descriptor of the certificate that shall be verified
 *  \return     E_OK                 certificate could be verified
 *  \return     E_NOT_OK             certificate could NOT be verified
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSignXxRsa(
    uint8 TConnId,
    uint8 HashAlg,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr )
{
  uint8   HashOutput[TLS_HASH_SHA256_LEN];  /* size fits for SHA1 and SHA256 */
  uint8   HashOutputLen;

  eslt_WorkSpaceRSAver  *RsaWorkSpPtr;  /* SHA1 WS? */
  const uint8  *PubKeyPtr;
  const uint8  *PubExpPtr;
  const uint8  *SignPtr;
  const uint8  *ContPtr;
  uint16        PubKeyLen;
  uint16        PubExpLen;
  uint16        SignLen;
  uint16        ContLen;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */

  RsaWorkSpPtr = (eslt_WorkSpaceRSAver*)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */

  /* get pub key pointer */
  PubKeyPtr = &CertValidPtr[CertValidDescPtr->KeyMain.ContOffs];
  PubKeyLen = CertValidDescPtr->KeyMain.ContLen;
  PubExpPtr = &CertValidPtr[CertValidDescPtr->KeyExp.ContOffs];
  PubExpLen = CertValidDescPtr->KeyExp.ContLen;

  /* get signature pointer */
  SignPtr = &CertToCheckPtr[CertToCheckDescPtr->CertSignature.ContOffs];
  SignLen = CertToCheckDescPtr->CertSignature.ContLen;

  /* get content pointer (for hashing) */
  ContPtr = &CertToCheckPtr[CertToCheckDescPtr->CertCh1.ContOffs];  /* signed certificate (ch1.1) including type and lenth field */
  ContLen = (uint16)(CertToCheckDescPtr->SignedCert.ContLen + (CertToCheckDescPtr->SignedCert.ContOffs - CertToCheckDescPtr->CertCh1.ContOffs));

  /* hash content */
  if(TLS_HASH_ALGORITHM_SHA1 == HashAlg)
  {
    /* reuse the ECC workspace for SHA */
    eslt_WorkSpaceSHA1 *ShaWorkSpPtr = (eslt_WorkSpaceSHA1*)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
    (void)esl_initWorkSpaceHeader(&ShaWorkSpPtr->header, ESL_MAXSIZEOF_WS_SHA1, NULL_PTR);
    (void)esl_initSHA1(ShaWorkSpPtr);
    (void)esl_updateSHA1(ShaWorkSpPtr, ContLen, ContPtr);
    (void)esl_finalizeSHA1(ShaWorkSpPtr, &HashOutput[0]);
    HashOutputLen = TLS_HASH_SHA1_LEN;
  }
  else if(TLS_HASH_ALGORITHM_SHA256 == HashAlg)
  {
    /* reuse the ECC workspace for SHA256 */
    eslt_WorkSpaceSHA256 *ShaWorkSpPtr = (eslt_WorkSpaceSHA256*)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
    (void)esl_initWorkSpaceHeader(&ShaWorkSpPtr->header, ESL_MAXSIZEOF_WS_SHA256, NULL_PTR);
    (void)esl_initSHA256(ShaWorkSpPtr);
    (void)esl_updateSHA256(ShaWorkSpPtr, ContLen, ContPtr);
    (void)esl_finalizeSHA256(ShaWorkSpPtr, &HashOutput[0]);
    HashOutputLen = TLS_HASH_SHA256_LEN;
  }
  else
  {
    /* hash algorithm not supported */
    return E_NOT_OK;
  }

  /* verify signature */
  return Crypto_ValidateRsaSignature(RsaWorkSpPtr, SignPtr, PubKeyPtr, PubExpPtr, &HashOutput[0], SignLen, PubKeyLen, PubExpLen, HashOutputLen, HashAlg);

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6050 */ /* MD_MSR_STCAL */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */

#endif

#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  Tls_VHs_VerifyCertSignXxEcdsa
 **********************************************************************************************************************/
/*! \brief      verifiy cert signature (sha1 with ecdsa)
 *  \param[in]  TConnId              TLS connection id
 *  \param[in]  HashAlg              Hash algorithm used for the cert signature
 *  \param[in]  CertValidPtr         pointer to the (assumed) valid certificate
 *  \param[in]  CertToCheckPtr       pointer to the certificate that shall be verified
 *  \param[in]  CertValidDescrPtr    pointer to the descriptor of the valid certificate
 *  \param[in]  CertToCheckDescrPtr  pointer to the descriptor of the certificate that shall be verified
 *  \return     E_OK                 certificate could be verified
 *  \return     E_NOT_OK             certificate could NOT be verified
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VHs_VerifyCertSignXxEcdsa(
    uint8 TConnId,
    uint8 HashAlg,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertValidPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) CertToCheckPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertValidDescPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertToCheckDescPtr )
{
  uint8   HashOutput[TLS_HASH_SHA256_LEN];  /* size fits for SHA1 and SHA256 */
  uint8   HashOutputLen;

  eslt_WorkSpaceEcP  *EcWorkSpPtr;
  const uint8  *PubKeyPtr;
  const uint8  *SignPtr;
  const uint8  *ContPtr;
  uint16        PubKeyLen;
  uint16        SignLen;
  uint16        ContLen;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */

  EcWorkSpPtr = (eslt_WorkSpaceEcP*)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */

  /* get pub key pointer */
  PubKeyPtr = &CertValidPtr[CertValidDescPtr->KeyMain.ContOffs];
  PubKeyLen = CertValidDescPtr->KeyMain.ContLen;

  /* get signature pointer */
  SignPtr = &CertToCheckPtr[CertToCheckDescPtr->CertSignature.ContOffs];
  SignLen = CertToCheckDescPtr->CertSignature.ContLen;

  /* get content pointer (for hashing) */
  ContPtr = &CertToCheckPtr[CertToCheckDescPtr->CertCh1.ContOffs];  /* signed certificate (ch1.1) including type and lenth field */
  ContLen = (uint16)(CertToCheckDescPtr->SignedCert.ContLen + (CertToCheckDescPtr->SignedCert.ContOffs - CertToCheckDescPtr->CertCh1.ContOffs));

  /* hash content */
  if(TLS_HASH_ALGORITHM_SHA1 == HashAlg)
  {
    /* reuse the ECC workspace for SHA */
    eslt_WorkSpaceSHA1 *ShaWorkSpPtr = (eslt_WorkSpaceSHA1*)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
    (void)esl_initWorkSpaceHeader(&ShaWorkSpPtr->header, ESL_MAXSIZEOF_WS_SHA1, NULL_PTR);
    (void)esl_initSHA1(ShaWorkSpPtr);
    (void)esl_updateSHA1(ShaWorkSpPtr, ContLen, ContPtr);
    (void)esl_finalizeSHA1(ShaWorkSpPtr, &HashOutput[0]);
    HashOutputLen = TLS_HASH_SHA1_LEN;
  }
  else if(TLS_HASH_ALGORITHM_SHA256 == HashAlg)
  {
    /* reuse the ECC workspace for SHA256 */
    eslt_WorkSpaceSHA256 *ShaWorkSpPtr = (eslt_WorkSpaceSHA256*)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
    (void)esl_initWorkSpaceHeader(&ShaWorkSpPtr->header, ESL_MAXSIZEOF_WS_SHA256, NULL_PTR);
    (void)esl_initSHA256(ShaWorkSpPtr);
    (void)esl_updateSHA256(ShaWorkSpPtr, ContLen, ContPtr);
    (void)esl_finalizeSHA256(ShaWorkSpPtr, &HashOutput[0]);
    HashOutputLen = TLS_HASH_SHA256_LEN;
  }
  else
  {
    /* hash algorithm not supported */
    return E_NOT_OK;
  }

  /* verify signature */
  return Crypto_ValidateEcdsaSignature(EcWorkSpPtr, SignPtr, PubKeyPtr, &HashOutput[0], SignLen, PubKeyLen, HashOutputLen, TRUE);
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6050 */ /* MD_MSR_STCAL */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */

#endif

#if (TLS_SUPPORT_OCSP == STD_ON)
/***********************************************************************************************************************
 *  Tls_VHs_RxServerCertStatus
 **********************************************************************************************************************/
/*! \brief      Receive server certificate status (OCSP stapling)
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \return                    consumed length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerCertStatus(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr )
{
  uint32 StatusMsgLen;

  if(DataPtr[0] == TLS_CERT_STAUS_TYPE_OCSP)
  {
    uint32 ResponsLen;
    Tls_Ocsp_RespDescrType StatusDescr;
    uint8 *ResponseMsgPtr;

    Tls_OcspWorkingMode[TConnId] = TLS_OCSP_MODE_USE_OCSP_STD;
    TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: OCSP STANDARD is used");
    TLS_GET_UINT24(DataPtr, 1, ResponsLen);

    StatusMsgLen = 1 + (3 + ResponsLen);
    TLS_LOG_HS_DIV_2("Tls_VHs_RxServerCertStatus: OCSP StatusMsgLen is %d, containing ResponsLen %d", StatusMsgLen, ResponsLen);
    ResponseMsgPtr = &DataPtr[4];

    /* now check the OCSP-Response (the whole following element is the OCSP-Response) */
    {
      Std_ReturnType RetVal;
      RetVal = Tls_VParseCertStatus(&StatusDescr, ResponseMsgPtr, (uint16)ResponsLen);
#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
      if(StatusDescr.R1CertStatusValue == TLS_STATUS_VALUE_GOOD)
      {
        /* cert has status 'good' */
        Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[TConnId], TLS_E_OCSP_RESP_NOT_GOOD, FALSE);  /* server cert has status 'good' in the OCSP response */
      }
      else
      {
        /* cert does not have status 'good' */
        Scc_StateM_Cbk_Tls_ErrorBitIndication((Tls_SocketIdType)Tls_CfgTlsConn2SockHndMap[TConnId], TLS_E_OCSP_RESP_NOT_GOOD, TRUE);  /* server cert has status other than 'good' in the OCSP response */
      }
#endif
      if(E_OK != RetVal)
      {
        /* error found while parsing the status message */
        TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: parsing status message resulted in an error");
        return 0;
      }
      else
      {
        TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: parsed status message successfully");
      }
    }

    /* check if there is a certificate included */
    if((StatusDescr.RespFoundElements & TLS_STATUS_ELE_CERTS) != 0)
    {
      /* there is a cert included in the OCSP response */
    }

#if (TLS_SUPPORT_REAL_TIME_CLOCK == STD_ON)
    /* ******************************************************************************************************************************************* */
    /* check if response can be accepted or if it is too old */
    {
      uint32                   Time32ProducedAt;
      uint32                   Time3ThisUdate;
      uint32                   Time32Now;
      sint32                   TimeDiffS;
      CONST(uint32, TLS_CONST) TempMandatoryStatusElements = ( TLS_STATUS_ELE_R1_THIS_UPDATE | TLS_STATUS_ELE_PRODUCED_AT );

      if((StatusDescr.RespFoundElements & TempMandatoryStatusElements) != TempMandatoryStatusElements)
      {
        /* error, mandatory element is missing */
        return 0;
      }

      /* get current (system) time */
      Tls_GetTime32Fct(&Time32Now);
      /* read time from status message */
      Tls_VReadCertTimeInfo( &(ResponseMsgPtr[StatusDescr.ProducedAt.ContOffs]),   (uint8)(StatusDescr.ProducedAt.ContLen),   &Time32ProducedAt );
      Tls_VReadCertTimeInfo( &(ResponseMsgPtr[StatusDescr.R1ThisUpdate.ContOffs]), (uint8)(StatusDescr.R1ThisUpdate.ContLen), &Time3ThisUdate );

      /* compare with validity start and end */
      TimeDiffS = (sint32)(Time32Now - Time3ThisUdate);
      if(TimeDiffS <= (sint32)(Tls_CfgOcspMaxResponseAgeS[TConnId] + TLS_CFG_ASSUMED_TIME_PRECISSION_S))
      {
        /* OK */
        TLS_LOG_HS_DIV_2("Tls_VHs_RxServerCertStatus: status message is new enough, the value 'thisUpdate' is ok, TimeDiff is %d and assumed TimePrecission is %d", TimeDiffS, TLS_CFG_ASSUMED_TIME_PRECISSION_S);
      }
      else
      {
        /* NOT_OK */
        TLS_LOG_HS_DIV_2("Tls_VHs_RxServerCertStatus: error, status message is too old, TimeDiff is %d and assumed TimePrecission is %d", TimeDiffS, TLS_CFG_ASSUMED_TIME_PRECISSION_S);
        /* Error, the status response is older than allowed, so the connection must be refused. */
        return 0;
      }

      /* verify that timestamp 'producedAt' is not older than timestamp 'thisUpdate' */
      if(Time32ProducedAt >= Time3ThisUdate)
      {
        /* OK */
        TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: ok, 'producedAt' is not older than 'thisUpdate'");
      }
      else
      {
        /* NOT_OK */
        /* Error, the time when this answer was produced ('producedAt') is older than the time when the latest status update was known to the OCSP server ('thisUpdate'). */
        /* 'thisUpdate' is the most recent time at which the status being indicated is known by the responder to have been correct. */
        /* 'producedAt' is the time at which the OCSP responder signed this response. */
        /* 'producedAt' must not be older than 'thisUpdate', otherwise the response is in an incosistent state and must be interpreted as invalid. */
        TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: error in ocsp-response, 'producedAt' is older than 'thisUpdate'");
        return 0;
      }
    }
#endif

    /* ******************************************************************************************************************************************* */
    /* check if the serial number in the response matches the serial number in the server certificate */
    if(FALSE == Tls_VOcspCheckSerialInServerCert(TConnId, &StatusDescr, ResponseMsgPtr))
    {
      TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: error in ocsp-response, serial number in the status message does not match with the server cert.");
      return 0;
    }

    /* ******************************************************************************************************************************************* */
    /* check if response is signed correctly */
    {
      /*
         OCSP_RESPONDER CERT ------------ CERTIFICATE_AUTHORITY CERT
              (OCSP-R)                             (CA)
                                                    |
                                                    |
                                                SERVER CERT

        - check if OCSP_RESPONDER and CERTIFICATE_AUTHIRITY are separate instances or if both use the same cert (CA).
      */
      CONST(uint32, TLS_CONST) MandatoryResponseElements = (TLS_STATUS_ELE_SIGN_ALG_ID | TLS_STATUS_ELE_SIGNATURE | TLS_STATUS_ELE_TBS_RESP_DATA);

      uint8                   *OcspResponseSignPtr;
      uint16                   OcspResponseSignLen;
      uint16                   OcspResponseSignAlg;

      uint32                  *WsPtr;
      uint8                    HashArray[TLS_HASH_SHA256_LEN];  /* maximum supported hash length */
      uint8                    HashLen;

      uint8                   *InclCertPtr = NULL_PTR;  /* This pointer is only initialized to avoid compiler warnings. The value is always set properly before usage. */
      uint16                   InclCertLen;
      Tls_Cert_Descr           InclCertDescr;

      uint8                   *ResponseRelatedPubKeyPtr;
      uint16                   ResponseRelatedPubKeyLen;

      uint8          *CaPubKeyPtr;
      uint16          CaPubKeyLen;
      uint16 CaRelatedSignAlg;

      /* set workspace pointer for the following hash and signature calculations */
      WsPtr = &Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]];

      if((StatusDescr.RespFoundElements & MandatoryResponseElements) != MandatoryResponseElements)
      {
        TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: error");  /* error */
        return 0;
      }

      /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      /* read signature algorithm id from OCSP response */
      if((StatusDescr.RespFoundElements & TLS_STATUS_ELE_SIGN_ALG_ID) != TLS_STATUS_ELE_SIGN_ALG_ID)
      {
        TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: error, signature id element not found");  /* error */
        return 0;
      }
      OcspResponseSignAlg = Tls_VHs_ReadSignAlgId(&ResponseMsgPtr[StatusDescr.SignAlgId.ContOffs], (uint8)StatusDescr.SignAlgId.ContLen);
      if(OcspResponseSignAlg == TLS_HASH_SIGN_ALG_ID_INVALID)
      {
        /* error, invalid or not supported signature algorithm id */
        TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: error");
        return 0;
      }

      /* read signature position in ocsp response */
      OcspResponseSignPtr = &(ResponseMsgPtr[StatusDescr.Signature.ContOffs]);
      OcspResponseSignLen = StatusDescr.Signature.ContLen;

      /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      /* hash the response data (TbsRespData), depending on the specified hash algorithm */
      switch(OcspResponseSignAlg)
      {
      case TLS_HASH_SIGN_ALG_ID_SHA1_ECDSA:
        {
          eslt_WorkSpaceSHA1 *ShaWsPtr;
          HashLen   = TLS_HASH_SHA1_LEN;
          ShaWsPtr  = (eslt_WorkSpaceSHA1*)(WsPtr);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
          (void)esl_initWorkSpaceHeader(&(ShaWsPtr->header), ESL_MAXSIZEOF_WS_SHA1, NULL_PTR);

          (void)esl_initSHA1(ShaWsPtr);
          (void)esl_updateSHA1(ShaWsPtr, StatusDescr.TbsRespDataFull.ContLen, &(ResponseMsgPtr[StatusDescr.TbsRespDataFull.ContOffs]));
          (void)esl_finalizeSHA1(ShaWsPtr, &HashArray[0]);
          break;
        }
      case TLS_HASH_SIGN_ALG_ID_SHA256_ECDSA:
        {
          eslt_WorkSpaceSHA256 *ShaWsPtr;
          HashLen   = TLS_HASH_SHA256_LEN;
          ShaWsPtr  = (eslt_WorkSpaceSHA256*)(WsPtr);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
          (void)esl_initWorkSpaceHeader(&(ShaWsPtr->header), ESL_MAXSIZEOF_WS_SHA256, NULL_PTR);

          (void)esl_initSHA256(ShaWsPtr);
          (void)esl_updateSHA256(ShaWsPtr, StatusDescr.TbsRespDataFull.ContLen, &(ResponseMsgPtr[StatusDescr.TbsRespDataFull.ContOffs]));
          (void)esl_finalizeSHA256(ShaWsPtr, &HashArray[0]);
          break;
        }
      default:
        {
          /* Error, not supported signature algorithm. Currently RSA signatures (for OCSP) are not supported. */
          TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: error, the ocsp response is signed with a signature/hash algorithm combination that is not supported");
          return 0;
        }
      }

      /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      /* There are two cases that have to be distinguished:
         a) The ocsp response is signed by the same CA as the certificate the ocsp response is for.
         b) The ocsp response is signed using a special certificate for the ocsp-responder. This ocsp-responder cert is signed by the same CA as the certificate the ocsp response is for.
         Depending on which case is needed for the current ocsp response the handling is done in different ways.
      */

      /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
      /* Validate the signature of the ocsp response using the public key from the included certificate, depending on the specified hash algorithm */
      if((StatusDescr.RespFoundElements & TLS_STATUS_ELE_CERTS) != 0)
      {
        InclCertPtr = &ResponseMsgPtr[StatusDescr.Cert1.ContOffs];
        InclCertLen = StatusDescr.Cert1.ContLen;

        /* parse the included certificate */
        if(E_OK != Tls_VParseServerCert(&InclCertDescr, TLS_HASH_INFO_ISSUER, InclCertPtr, InclCertLen))
        {
          /* parsing the certificate failed */
          return 0;
        }

            /* all signature algorithms using ECDSA */
        ResponseRelatedPubKeyPtr = &InclCertPtr[InclCertDescr.KeyMain.ContOffs];
        ResponseRelatedPubKeyLen = InclCertDescr.KeyMain.ContLen;

        /* verify signature */
        if(E_OK != Crypto_ValidateEcdsaSignature((eslt_WorkSpaceEcP*)(WsPtr), OcspResponseSignPtr, ResponseRelatedPubKeyPtr, &HashArray[0], OcspResponseSignLen, ResponseRelatedPubKeyLen, HashLen, TRUE))  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
        {
          TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: Error! The validation of the signature of the OCSP against the inlcude OCSP-Responder certificate failed");  /* error */
          return 0;
        }
        else
        {
          TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: OK. The signature of the OCSP response was successfully validated against the inlcude OCSP-Responder certificate");
        }

        CaRelatedSignAlg = Tls_VHs_ReadSignAlgId(&InclCertPtr[InclCertDescr.CertSignAlgId.ContOffs], (uint8)InclCertDescr.CertSignAlgId.ContLen);
      }
      else
      {
        CaRelatedSignAlg = OcspResponseSignAlg;
      }

      /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
        /* Take the public key of the issuing CA to validate the signature of the included certificate. */
      /* Validate the signature of the included certificate (if available) or the signature of the ocsp response against the matching CA. */
      if((CaRelatedSignAlg == TLS_HASH_SIGN_ALG_ID_SHA1_ECDSA) || (CaRelatedSignAlg == TLS_HASH_SIGN_ALG_ID_SHA256_ECDSA))
      {
        uint8          *CertToCheckPtr;
        Tls_Cert_Descr *CertToCheckDescriptorPtr;
        uint8          *CertValidPtr;
        Tls_Cert_Descr  Reduced_CertValidDescriptor;
        Tls_Cert_Descr *CertValidDescriptorPtr;

        if(Tls_Ocsp_CertChainLen[TConnId] == 1)
        {
          /* the cert chain length is 1 (there is no chain), so the cert has to be validated against a stored root certificate */
          uint8_least RootCertIdx;

          for(RootCertIdx = 0; RootCertIdx < TLS_CFG_NUM_SERVER_CERTS; RootCertIdx++)
          {
            if(Tls_CfgServerCertBlockIds[RootCertIdx] == Tls_UsedRootCertNvmBlockId[TConnId])
            {
              /* found the matching root cert */
              break;
            }
          }

          CertValidDescriptorPtr = &Tls_ServerCertDescs[RootCertIdx];
          CertValidPtr           = &Tls_NvmExchData[0];  /* Tls_NvmExchData currently already contains the matching root certificate */
          CaPubKeyPtr            = &CertValidPtr[CertValidDescriptorPtr->KeyMain.ContOffs];
          CaPubKeyLen            = CertValidDescriptorPtr->KeyMain.ContLen;
        }
        else
        {
          /* the cert chain length is longer than 1, so the cert in the ocsp response has to be validated against a certificate from the cert chain received in the server_certificate message */

          CertValidDescriptorPtr = &Reduced_CertValidDescriptor;
          /* take the key of the cert that issued the server (leaf) cert */
          /* For ocsp_multi the correct cert/key has to be chosen (not yet implemented). */
          CertValidPtr           = &Tls_Ocsp_CertChainKeys[((TConnId * (TLS_CFG_OCSP_MAX_CERT_CHAIN_LEN -1)) + 0) * TLS_CFG_MAX_SERVER_CERT_KEY_SIZE_BYTE];
          CaPubKeyPtr            = &Tls_Ocsp_CertChainKeys[((TConnId * (TLS_CFG_OCSP_MAX_CERT_CHAIN_LEN -1)) + 0) * TLS_CFG_MAX_SERVER_CERT_KEY_SIZE_BYTE];
          /* currently the only supported curve is prime256r1, so the key length can be hard-coded */
          CaPubKeyLen            = (2 * TLS_ECC_P256R1_COORDINATE_SIZE) + 1;  /* CertValidDescriptorPtr->KeyMain.ContLen; */

          Reduced_CertValidDescriptor.KeyMain.ContLen  = 0x41;  /* Ca_cert_descr_ptr->KeyMain.ContLen; */
          Reduced_CertValidDescriptor.KeyMain.ContOffs = 0;
        }

        if((StatusDescr.RespFoundElements & TLS_STATUS_ELE_CERTS) != 0)
        {
          /* signature of the included cert has to be validated */
          CertToCheckPtr = InclCertPtr;  /* PRQA S 3353 */ /* MD_TLS_9.1_3353_a */  /*lint !e644 */ /* 'InclCertPtr' is always initialized for the current condition */
          CertToCheckDescriptorPtr = &InclCertDescr;

          /* The trick to use a fake certificate and descriptor will not work for RSA, since then two elements
          (key + exponent) are needed with a propper relation in the memory. Eventually this might work if some
          pointer-arithmetic is used to find out the absolute position of one element (the RSA modulus) and the
          relative position of the other one (the RSA exponent). */
          if(E_OK != Tls_VHs_VerifyCertSign(TConnId, CertValidPtr, CertToCheckPtr, CertValidDescriptorPtr, CertToCheckDescriptorPtr))
          {
            TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: error, validation of received ocsp-responder cert against stored CA cert failed");
            return 0;
          }
          else
          {
            TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: OK, validation of received ocsp-responder cert against stored CA cert succeeded");
          }

        }
        else
        {
          /* signature of the ocsp response has to be validated */
          uint8 *SignPtr = &(ResponseMsgPtr[StatusDescr.Signature.ContOffs]);
          uint16 SignLen = StatusDescr.Signature.ContLen;
          if(E_OK == Crypto_ValidateEcdsaSignature((eslt_WorkSpaceEcP*)WsPtr, SignPtr, CaPubKeyPtr, &HashArray[0], SignLen, CaPubKeyLen, HashLen, TRUE))  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
          {
            TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: ok, validation of received ocsp response against a CA succeeded");
          }
          else
          {
            TLS_LOG_HS_DIV("Tls_VHs_RxServerCertStatus: error, validation of received ocsp response against a CA failed");
            return 0;
          }
        }
      }
      else
      {
        /* invalid or not supported signature algorithm */
      }

    }

  }
  else if(DataPtr[0] == TLS_CERT_STAUS_TYPE_OCSP_MULTI)  /* ------------------------------------------------------------------ */
  {
    /* error, handling multiple OCSP statur responses is not yet implemented */
    return 0;
  }
  else
  {
    /* error, invalid status type value */
    return 0;
  }

  return (uint16)StatusMsgLen;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VOcspCheckSerialInServerCert
 **********************************************************************************************************************/
/*! \brief      Check if the serial number in the response matches the serial number in the server certificate.
 *  \param[in]  TConnId         TLS connection id
 *  \param[in]  StatusDescrPtr  pointer to the status descriptor for the received status message
 *  \param[in]  ResponseMsgPtr  pointer to the received status message
 *  \return     TRUE            The two serial numbers are equal
 *  \return     FALSE           The two serial numbers are not equal or an other error occurred
 *  \context    TASK
 **********************************************************************************************************************/
STATIC FUNC(boolean, TLS_CODE) Tls_VOcspCheckSerialInServerCert(
    uint8 TConnId,
    P2CONST(Tls_Ocsp_RespDescrType, AUTOMATIC, TLS_CONST) StatusDescrPtr,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) ResponseMsgPtr )
{
  P2CONST(uint8, AUTOMATIC, TLS_CONST) serverCertSerialPtr;
  P2CONST(uint8, AUTOMATIC, TLS_CONST) statusSerialPtr;
  uint8                                serverCertSerialLen;
  uint8                                statusSerialLen;
  boolean                              serialCheckResultOk = TRUE;

  /* # Check if the serial number element was found in the status message */
  if((StatusDescrPtr->RespFoundElements & (uint32)TLS_STATUS_ELE_R1_CID_SERIAL_NUMBER) == 0)
  {
    /* error, mandatory element is missing */
    serialCheckResultOk = FALSE;
  }
  else
  {
    /* parse server cert to find its serial number */
    uint32                                  certLen;  /* Length of the certificate. */
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA)  certPtr = &Tls_HsServerCertChainPtr[TConnId][TLS_HS_S_TLS_CERT_LEN_LEN];
    uint8                                   certVer;  /* '1' or '2' (version 2 and 3 have the same structure) */
    CONST(uint8, TLS_CONST)                 certTable[2][TLS_HS_CERT_PARSING_DEPTH_SERIAL] =  /* '0'-values are invalid */
    {
      { 1, 1, 1 },  /* chapter 1.1.1: cert serial number */  /* v1 */
      { 1, 1, 2 }   /* chapter 1.1.2: cert serial number */  /* v3 */
    };
    IpBase_BerWorkspaceType                 berWorkspace;
    IpBase_BerStackElementType              berStack[TLS_HS_CERT_PARSING_DEPTH_SERIAL];  /* max depth of one element */
    IpBase_BerElementType                   berEleDesc;

    /* # Read the length of the certificate */
    TLS_GET_UINT24(Tls_HsServerCertChainPtr[TConnId], 0, certLen);

    /* Init the BER decoding workspace once */
    IpBase_BerInitWorkspace(&berWorkspace, &berStack[0], TLS_HS_CERT_PARSING_DEPTH_SERIAL);

    /* # Find the position of the first element in the certificate's 'signed certificate' chapter */
    /* This certificate has already been parsed for this element in an earlier stage and would have caused an error
    there, so parsing for this element now can not produce an error. */
    (void)IpBase_BerGetElement(&berWorkspace, &berEleDesc, &certTable[0][0], TLS_HS_CERT_PARSING_DEPTH_SERIAL, certPtr, certLen);

    /* # Read the first byte of the signed certificate chapter to find out wether the next sub-chapter is the
    serial number or the version information. */
    /* The first byte descibes the class of the next BER-encoded chapter: either it is 0x02 (integer value) and
    therefor the serial number, or it is 0xA0 (optional element) and therefore the certificate version field */
    if(0x02 == certPtr[berEleDesc.Idx])
    {
      /* v1 certificate: first element is the serial number, there is no version number  */
      certVer = 1;
    }
    else
    {
      /* v2 or v3 certificate: first element is the version number */
      certVer = 2;
    }

    /* # Find the position of the serial number if it isn't found already */
    if(certVer == 1)
    {
      /* serial element already found in the previous step */
    }
    else
    {
      /* parse for the serial number element */
      (void)IpBase_BerGetElement(&berWorkspace, &berEleDesc, &certTable[1][0], TLS_HS_CERT_PARSING_DEPTH_SERIAL, certPtr, certLen);
      /* This certificate has already been parsed for this element in an earlier stage and would have caused an
      error there, so parsing for this element now can not produce an error. */
    }

    serverCertSerialPtr = &certPtr[berEleDesc.ContentIdx];
    serverCertSerialLen =  (uint8)(berEleDesc.EndIdx - berEleDesc.ContentIdx);
    statusSerialPtr     = &ResponseMsgPtr[StatusDescrPtr->R1Cid_serialNumber.ContOffs];
    statusSerialLen     =          (uint8)StatusDescrPtr->R1Cid_serialNumber.ContLen;

    /* # Compare the two serial numbers. */
    if(serverCertSerialLen == statusSerialLen)
    {
      if(IPBASE_CMP_EQUAL == IpBase_StrCmpLen(serverCertSerialPtr, statusSerialPtr, serverCertSerialLen))
      {
        /* both serial numbers are equal */
      }
      else
      {
        /* serial numbers have the same length but are not equal */
        serialCheckResultOk = FALSE;
      }
    }
    else
    {
      /* serial numbers have different lengths and therefore are not equal */
      serialCheckResultOk = FALSE;
    }
  }

  return serialCheckResultOk;
}

/***********************************************************************************************************************
 *  Tls_VParseCertStatus
 **********************************************************************************************************************/
/*! \brief      Parse status message that was received from the server
 *  \param[out] StatusDescr    pointer where the status information shall be stored
 *  \param[in]  DataPtr        pointer to the status message
 *  \param[in]  DataLen        length of the status message
 *  \return     E_OK           status message could be parsed
 *  \return     E_NOT_OK       status message could not be parsed
 *  \note       This function is similar to Tls_VParseServerCert
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VParseCertStatus(
    CONSTP2VAR(Tls_Ocsp_RespDescrType, AUTOMATIC, TLS_CONST) StatusDescr,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen )
{
  const uint8 *BasicOcspRespPtr;  /* pointer to the basic OCSP response */
  const uint8 *Response1Ptr;      /* pointer to the first response element inside the basic OCSP response */
  uint16 BasicOcspRespLen;
  uint16 Response1Len;
  uint16 BasicOcspRespOffset;
  uint16 Response1Offset;

  StatusDescr->RespFoundElements = 0;

  if(0 == DataPtr[0])
  {
    /* invalid input */
    return E_NOT_OK;
  }

  /* now parse the certificate */
  {
    CONST(uint8, TLS_CONST) EleNrResponse[1]        = { 1 };           /* chapter 1          Response */
    CONST(uint8, TLS_CONST) EleNrResponseStatus[2]  = { 1, 1 };        /* chapter 1.1        ResponseStatus */
    CONST(uint8, TLS_CONST) EleNrRespTypeOid[4]     = { 1, 2, 1, 1 };  /* chapter 1.2.1.1    ResponseType ID */
    CONST(uint8, TLS_CONST) EleNrBasicOcspResp[4]   = { 1, 2, 1, 2 };  /* chapter 1.2.1.2    basicOCSPresponse */

    /* Sub-chapters of the element BasicOcspResp (chapter 1.2.1.2). This is an internal coding not 'connected' to the outer coding. */
    CONST(uint8, TLS_CONST) BasicOcspRespTable[9][4] =  /* '0'-values are invalid */
    {
      { 1, 1, 0, 0 },  /* chapter 1.2.1.2___1.1       tbsResponseData */
      { 1, 1, 1, 0 },  /* chapter 1.2.1.2___1.1.1     responderId */
      { 1, 1, 2, 0 },  /* chapter 1.2.1.2___1.1.2     producedAt */
      { 1, 1, 3, 0 },  /* chapter 1.2.1.2___1.1.3     responses */
      { 1, 1, 3, 1 },  /* chapter 1.2.1.2___1.1.3.1   response1 */
      { 1, 2, 0, 0 },  /* chapter 1.2.1.2___1.2       signatureAlgorithm */
      { 1, 3, 0, 0 },  /* chapter 1.2.1.2___1.3       signature */
      { 1, 4, 0, 0 },  /* chapter 1.2.1.2___1.4       certs */
      { 1, 4, 1, 0 },  /* chapter 1.2.1.2___1.4.1     cert1 */
    };

    /* Sub-chapters of the element Response1 (chapter 1.2.1.2___1.1.3.1). These are normal sub-elements of the chapter 'Response1', but to organize the data it is handled separately. */
    /* LIMITATION: The stucture is made to support more than one reply, but we will not implement a support for multiple answers from one single OCSP responder */
    CONST(uint8, TLS_CONST) ResponseXTable[9][3] =  /* '0'-values are invalid */
    {
      { 1, 0, 0 },  /* chapter 1.2.1.2___1.1.3.1...1     r1_certID */
      { 1, 1, 0 },  /* chapter 1.2.1.2___1.1.3.1...1.1   r1_hashAlgorithm */
      { 1, 2, 0 },  /* chapter 1.2.1.2___1.1.3.1...1.2   r1_issuerNameHash */
      { 1, 3, 0 },  /* chapter 1.2.1.2___1.1.3.1...1.3   r1_issuerKeyHash */
      { 1, 4, 0 },  /* chapter 1.2.1.2___1.1.3.1...1.4   r1_serialNumber */
      { 2, 0, 0 },  /* chapter 1.2.1.2___1.1.3.1...2     r1_certStatus */
      { 2, 1, 0 },  /* chapter 1.2.1.2___1.1.3.1...2.1   r1_revocationTime */  /* only for status 'revoked' */
      { 3, 0, 0 },  /* chapter 1.2.1.2___1.1.3.1...3     r1_thisUpdate */
      { 4, 0, 0 },  /* chapter 1.2.1.2___1.1.3.1...4     r1_nextUpdate */  /* OPTIONAL */
    };

    CONST(uint8, TLS_CONST) MaxEleDepth           = 4;

    IpBase_ReturnType           Res;
    IpBase_BerWorkspaceType     Workspace;
    IpBase_BerStackElementType  BerStack[4];  /* max depth of one element */
    IpBase_BerElementType       EleDesc;
    uint32                      LocDataLen = DataLen;  /* max */

    IpBase_BerInitWorkspace(&Workspace, &BerStack[0], MaxEleDepth); /* only needed once */

    /* parse for chapter 1          Response */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrResponse[0], 1, DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      LocDataLen = EleDesc.EndIdx - EleDesc.Idx;
      StatusDescr->Response.ContOffs = (uint16)EleDesc.ContentIdx;
      StatusDescr->Response.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_RESPONSE;

      if(EleDesc.EndIdx != DataLen)
      {
        return E_NOT_OK;  /* the length of the whole response is known to the calling function, a missmatch is not allowed */
      }
    }
    else
    {
      return E_NOT_OK;
    }

    /* parse for chapter 1.1        ResponseStatus */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrResponseStatus[0], 2, DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->ResponseStatus.ContOffs = (uint16)EleDesc.ContentIdx;
      StatusDescr->ResponseStatus.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_RESPONSE_STATUS;

      if(DataPtr[EleDesc.ContentIdx] != 0x00U)  /* check if status is 'successful' */
      {
        return E_NOT_OK;  /* the response status is not 'successful', so stop the further parsing of the status message */
      }
    }
    else
    {
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.1    ResponseType ID */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrRespTypeOid[0], 4, DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      CONST(uint8, TLS_CONST) oidRef[11] = TLS_OID_OCSP_RESP_TYPE_OCSP_BASIC;
      /* now, EleDesc contains the index information */
      StatusDescr->RespTypeOid.ContOffs = (uint16)EleDesc.ContentIdx;
      StatusDescr->RespTypeOid.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_RESP_TYPE_OID;

      if(IPBASE_CMP_EQUAL != IpBase_StrCmpLen(&oidRef[0], &DataPtr[EleDesc.Idx], 11))  /* check if OID is 'ocsp-basic' */
      {
        return E_NOT_OK;  /* not supported response type */
      }
    }
    else
    {
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2    basicOCSPresponse */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrBasicOcspResp[0], 4, DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->BasicOcspResp.ContOffs = (uint16)EleDesc.ContentIdx;
      StatusDescr->BasicOcspResp.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_BASIC_OCSP_RESP;

      BasicOcspRespPtr    = &DataPtr[EleDesc.ContentIdx];
      BasicOcspRespLen    = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      BasicOcspRespOffset = (uint16)EleDesc.ContentIdx;
    }
    else
    {
      return E_NOT_OK;
    }

    /* ------------------------------------------------------------------------------------------------------------- */
    /* ------------------------------------------------------------------------------------------------------------- */

    /* parse for chapter 1.2.1.2___1.1   tbsResponseData, plus one element including the length info */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[0][0], 2, BasicOcspRespPtr, BasicOcspRespLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->TbsRespData.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset;
      StatusDescr->TbsRespData.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_TBS_RESP_DATA;

      StatusDescr->TbsRespDataFull.ContOffs = (uint16)EleDesc.Idx + BasicOcspRespOffset;
      StatusDescr->TbsRespDataFull.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.Idx);
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at tbsResponseData");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.1   responderId */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[1][0], 3, BasicOcspRespPtr, BasicOcspRespLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->ResponderId.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset;
      StatusDescr->ResponderId.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_RESPONDER_ID;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at responderId");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.2   producedAt */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[2][0], 3, BasicOcspRespPtr, BasicOcspRespLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->ProducedAt.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset;
      StatusDescr->ProducedAt.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_PRODUCED_AT;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at producedAt");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.3   responses */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[3][0], 3, BasicOcspRespPtr, BasicOcspRespLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->Responses.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset;
      StatusDescr->Responses.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_RESPONSES;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at responses");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.3.1   response1 */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[4][0], 4, BasicOcspRespPtr, BasicOcspRespLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->Response1.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset;
      StatusDescr->Response1.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_RESPONSE_1;

      Response1Ptr    = &BasicOcspRespPtr[EleDesc.ContentIdx];
      Response1Len    = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      Response1Offset = (uint16)(EleDesc.ContentIdx + BasicOcspRespOffset);
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at response1");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.2   signatureAlgorithm */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[5][0], 2, BasicOcspRespPtr, BasicOcspRespLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->SignAlgId.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset;
      StatusDescr->SignAlgId.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_SIGN_ALG_ID;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at signatureAlgorithm");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.3   signature */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[6][0], 2, BasicOcspRespPtr, BasicOcspRespLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      uint8 ExtraOffset = 0;
      if((EleDesc.Type == 0x03) && (BasicOcspRespPtr[EleDesc.ContentIdx] == 0x0))
      {
        ExtraOffset = 1;
      }
      StatusDescr->Signature.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset + ExtraOffset;
      StatusDescr->Signature.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx - ExtraOffset);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_SIGNATURE;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at signature");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.4   certs */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[7][0], 2, BasicOcspRespPtr, BasicOcspRespLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->Certs.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset;
      StatusDescr->Certs.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_CERTS;

      /* parse for chapter 1.2.1.2___1.4.1   cert1 */  /* -------------------------------------------------- */
      Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BasicOcspRespTable[8][0], 3, BasicOcspRespPtr, BasicOcspRespLen);
      if( IPBASE_E_OK == Res )
      {
        /* now, EleDesc contains the index information */
        StatusDescr->Cert1.ContOffs = (uint16)EleDesc.ContentIdx + BasicOcspRespOffset;
        StatusDescr->Cert1.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
        StatusDescr->RespFoundElements |= TLS_STATUS_ELE_CERT1;
      }
      else
      {
        TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at certs (optinal element)");
        return E_NOT_OK;
      }

    }
    else
    {
      /* optional element is missing */
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at certs (optinal element)");
    }

    /* ------------------------------------------------------------------------------------------------------------- */
    /* ------------------------------------------------------------------------------------------------------------- */

    /* parse for chapter 1.2.1.2___1.1.3.1...1   r1_certI */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[0][0], 1, Response1Ptr, Response1Len);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->R1CertId.ContOffs = (uint16)EleDesc.ContentIdx + Response1Offset;
      StatusDescr->R1CertId.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_R1_CERT_ID;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_certI");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.3.1...1.1   r1_hashAlgorithm */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[1][0], 2, Response1Ptr, Response1Len);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->R1Cid_HashAlg.ContOffs = (uint16)EleDesc.ContentIdx + Response1Offset;
      StatusDescr->R1Cid_HashAlg.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_R1_CID_HASH_ALG;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_hashAlgorithm");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.3.1...1.2   r1_issuerNameHash */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[2][0], 2, Response1Ptr, Response1Len);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->R1Cid_IssuerNameHash.ContOffs = (uint16)EleDesc.ContentIdx + Response1Offset;
      StatusDescr->R1Cid_IssuerNameHash.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_R1_CID_ISSU_NAME_HASH;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_issuerNameHash");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.3.1...1.3   r1_issuerKeyHash */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[3][0], 2, Response1Ptr, Response1Len);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->R1Cid_IssuerKeyHash.ContOffs = (uint16)EleDesc.ContentIdx + Response1Offset;
      StatusDescr->R1Cid_IssuerKeyHash.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_R1_CID_ISSU_KEY_HASH;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_issuerKeyHash");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.3.1...1.4   r1_serialNumber */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[4][0], 2, Response1Ptr, Response1Len);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->R1Cid_serialNumber.ContOffs = (uint16)EleDesc.ContentIdx + Response1Offset;
      StatusDescr->R1Cid_serialNumber.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_R1_CID_SERIAL_NUMBER;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_serialNumber");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.3.1...2   r1_certStatus */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[5][0], 1, Response1Ptr, Response1Len);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->R1CertStatus.ContOffs = (uint16)EleDesc.ContentIdx + Response1Offset;
      StatusDescr->R1CertStatus.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_R1_CERT_STATUS;

      StatusDescr->R1CertStatusValue = EleDesc.Type & 0x0F;

      if(StatusDescr->R1CertStatus.ContLen != 0)
      {
        /* revoked, eventually also evaluate time when cert was revoked */
      }

      if(StatusDescr->R1CertStatusValue == TLS_STATUS_VALUE_GOOD)
      {
        TLS_LOG_HS_DIV("Tls_VParseCertStatus: cert has status 'good'");
      }
      else if(StatusDescr->R1CertStatusValue == TLS_STATUS_VALUE_REVOKED)
      {
        TLS_LOG_HS_DIV("Tls_VParseCertStatus: cert has status 'revoked'");
        return 0;
      }
      else if(StatusDescr->R1CertStatusValue == TLS_STATUS_VALUE_UNKNOWN)
      {
        TLS_LOG_HS_DIV("Tls_VParseCertStatus: cert has status 'unknown'");
      }
      else
      {
        TLS_LOG_HS_DIV("Tls_VParseCertStatus: cert has an invalid value for status");
      }
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_certStatus");
      return E_NOT_OK;
    }

    if(StatusDescr->R1CertStatusValue == TLS_STATUS_VALUE_REVOKED)
    {
      /* parse for chapter 1.2.1.2___1.1.3.1...2.1   r1_revocationTime */  /* -------------------------------------------------- */
      Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[6][0], 2, Response1Ptr, Response1Len);
      if( IPBASE_E_OK == Res )
      {
        /* now, EleDesc contains the index information */
        StatusDescr->R1RevocationTime.ContOffs  = (uint16)EleDesc.ContentIdx + Response1Offset;
        StatusDescr->R1RevocationTime.ContLen   = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
        StatusDescr->RespFoundElements         |= TLS_STATUS_ELE_R1_REVOC_TIME;
      }
      else
      {
        TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_revocationTime");
        return E_NOT_OK;
      }
    }

    /* parse for chapter 1.2.1.2___1.1.3.1...3   r1_thisUpdate */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[7][0], 1, Response1Ptr, Response1Len);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->R1ThisUpdate.ContOffs = (uint16)EleDesc.ContentIdx + Response1Offset;
      StatusDescr->R1ThisUpdate.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_R1_THIS_UPDATE;
    }
    else
    {
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_thisUpdate");
      return E_NOT_OK;
    }

    /* parse for chapter 1.2.1.2___1.1.3.1...4   r1_nextUpdate  (OPTIONAL) */  /* -------------------------------------------------- */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &ResponseXTable[8][0], 1, Response1Ptr, Response1Len);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      StatusDescr->R1NextUpdate.ContOffs = (uint16)EleDesc.ContentIdx + Response1Offset;
      StatusDescr->R1NextUpdate.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      StatusDescr->RespFoundElements |= TLS_STATUS_ELE_R1_NEXT_UPDATE;
    }
    else
    {
      /* optional element is missing */
      TLS_LOG_HS_DIV("Tls_VParseCertStatus: parsing failed at r1_nextUpdate (optional element)");
    }

    /* ------------------------------------------------------------------------------------------------------------- */
    /* ------------------------------------------------------------------------------------------------------------- */

  }
  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

/***********************************************************************************************************************
 *  Tls_VHs_RxServerKeyExchange
 **********************************************************************************************************************/
/*! \brief      Receive server key exchange message (ECDHE)
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \return                    consumed length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerKeyExchange(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr )
{
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
  uint16 LocReadPos;
  uint16 EcPointPos;
  uint16 SignLen;
  uint8 *SignPtr;
  uint8  KeyDataLen;
  uint8  HashAlg;
  uint8* ServerPubKeyPtr;

  /* check curve id and format --------------------------------------------- */
  /* check curve type */
  if(0x03 != DataPtr[0])  /* curve type: named curve */
  {
    /* unsupported curve format */
    return 0;
  }

  /* check curve id */
  if((0x00 != DataPtr[1]) || (0x17 != DataPtr[2]))
  {
    /* curve other than prime256v1, this is not supported */
    return 0;
  }

  /* check point length */
  KeyDataLen = DataPtr[3];
  if(0x42 == KeyDataLen)
  {
    /* check the fill byte for beeing '0' */
    if(0 != DataPtr[4])
    {
      return 0;  /* error */
    }
    LocReadPos = 5;
  }
  else if(0x41 == KeyDataLen)
  {
    LocReadPos = 4;
  }
  else
  {
    return 0;  /* error, invalid length */
  }

  /* check point format */
  if(TLS_ECC_PUB_KEY_POINT_FORMAT_UNCOMPRESSED != DataPtr[LocReadPos])
  {
    /* unsupported point format */
    return 0;
  }
  EcPointPos = LocReadPos;  /* the key data is now at LocReadPos */
  LocReadPos++;

  /* check signature ------------------------------------------------------- */
  LocReadPos += Tls_ServEncKeyLen[TConnId];

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  if(TLS_PROT_VER_TLS12 == Tls_ProtVer[TConnId])
  {
    /* check hash and signature algorithm */

    if(TLS_HASH_ALGORITHM_SHA256 == DataPtr[LocReadPos])
    {
      HashAlg = TLS_HASH_ALGORITHM_SHA256;
    }
    else if(TLS_HASH_ALGORITHM_SHA1 == DataPtr[LocReadPos])
    {
      HashAlg = TLS_HASH_ALGORITHM_SHA1;
    }
    else
    {
      /* error, invalid hash algorithm */
      return 0;
    }

    LocReadPos += 2;
  }
  else
#endif
  {
    /* for TLS 1.0 it is always SHA1 */
    HashAlg = TLS_HASH_ALGORITHM_SHA1;
  }

  {
    uint8 HashOutput[TLS_HASH_SHA256_LEN];
    uint8 HashOutputLen;
    uint16 ContLen;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
    uint8* ContPtr;

    ContPtr = &DataPtr[0];
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    if(TLS_PROT_VER_TLS12 == Tls_ProtVer[TConnId])
    {
      ContLen = LocReadPos -2;
    }
    else
#endif
    {
      ContLen = LocReadPos;
    }
    SignLen = (uint16)(((uint16)(DataPtr[LocReadPos]) << 8) + DataPtr[LocReadPos+1]);
    SignPtr = &DataPtr[LocReadPos +2];

    /*
         select (SignatureAlgorithm) {
              case ecdsa:
                  digitally-signed struct {
                      opaque sha_hash[sha_size];
                  };
          } Signature;

        ServerKeyExchange.signed_params.sha_hash
            SHA(ClientHello.random + ServerHello.random + ServerKeyExchange.params);
    */
    /* hash content */
    /* reuse the ECC workspace for SHA */
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    if(TLS_HASH_ALGORITHM_SHA1 == HashAlg)
#endif
    {
      /* SHA1 */
      eslt_WorkSpaceSHA1 *ShaWorkSpPtr = (eslt_WorkSpaceSHA1*)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
      (void)esl_initWorkSpaceHeader(&ShaWorkSpPtr->header, ESL_MAXSIZEOF_WS_SHA1, NULL_PTR);
      (void)esl_initSHA1(ShaWorkSpPtr);
      (void)esl_updateSHA1(ShaWorkSpPtr, TLS_HS_RANDOM_LEN, &(Tls_SecParamTxPassivePtr[TConnId]->client_random[0]));
      (void)esl_updateSHA1(ShaWorkSpPtr, TLS_HS_RANDOM_LEN, &(Tls_SecParamTxPassivePtr[TConnId]->server_random[0]));
      (void)esl_updateSHA1(ShaWorkSpPtr, ContLen, ContPtr);
      (void)esl_finalizeSHA1(ShaWorkSpPtr, &HashOutput[0]);
      HashOutputLen = TLS_HASH_SHA1_LEN;
    }
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    else if(TLS_HASH_ALGORITHM_SHA256 == HashAlg)  /* PRQA S 3355,3358 */ /* MD_TLS_13.7_3355_3358 */
    {
      /* SHA256 */
      eslt_WorkSpaceSHA256 *ShaWorkSpPtr = (eslt_WorkSpaceSHA256*)(&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
      (void)esl_initWorkSpaceHeader(&ShaWorkSpPtr->header, ESL_MAXSIZEOF_WS_SHA256, NULL_PTR);
      (void)esl_initSHA256(ShaWorkSpPtr);
      (void)esl_updateSHA256(ShaWorkSpPtr, TLS_HS_RANDOM_LEN, &(Tls_SecParamTxPassivePtr[TConnId]->client_random[0]));
      (void)esl_updateSHA256(ShaWorkSpPtr, TLS_HS_RANDOM_LEN, &(Tls_SecParamTxPassivePtr[TConnId]->server_random[0]));
      (void)esl_updateSHA256(ShaWorkSpPtr, ContLen, ContPtr);
      (void)esl_finalizeSHA256(ShaWorkSpPtr, &HashOutput[0]);
      HashOutputLen = TLS_HASH_SHA256_LEN;
    }
    else
    {  /* PRQA S 3201 */ /* MS_MSR_14.1 */
      /* hash algorithm not supported */
      return 0;
    }
#endif

    /* verify signature */
    {
      eslt_WorkSpaceEcP *TmpWorkspacePtr;

      ServerPubKeyPtr = &Tls_ServEncKey[Tls_VCfgGetMaxServerCertKeySizeByte() * TConnId];

      TmpWorkspacePtr = (eslt_WorkSpaceEcP*)&Tls_DynCryptoWs[Tls_VCfgGetDynCryptoWsStartOffs()[TConnId]];  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */  /* PRQA S 3305 */ /* MD_TLS_3305_ws */
      if(E_OK != Crypto_ValidateEcdsaSignature( TmpWorkspacePtr, SignPtr, ServerPubKeyPtr, &HashOutput[0], SignLen, (Tls_ServEncKeyLen[TConnId] +1), HashOutputLen, TRUE))
      {
        /* error, cancel handshake */
        return 0;
      }
    }

  }

  /* copy point / key data ------------------------------------------------- */
  {
    uint8 *DestPtr = &Tls_ServEncKey[Tls_VCfgGetMaxServerCertKeySizeByte() * TConnId];
    /* copy pub key */
    IpBase_Copy( (IpBase_CopyDataType*)(DestPtr),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
      (const IpBase_CopyDataType*)(&(DataPtr[EcPointPos])),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
      Tls_ServEncKeyLen[TConnId] +1);
  }

  return (LocReadPos + 2 + SignLen);

#else
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_OFF)
      TLS_UNUSED_PARAMETER(TConnId)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
      TLS_UNUSED_PARAMETER(DataPtr)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  /* error, this messages should not have been received */
  return 0;
#endif
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  Tls_VHs_RxCertRequest
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxCertRequest(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr )
{
  uint16 locReadPos = 0;
  boolean processingSucceeded = TRUE;

  /* #10 Read and ignore the list of accepted certificate types (RSA, DSS, ...) */
  {
    /* The current implementation does not support using RSA certificates for ECDSA cipher suites and vice versa, so
       only a matching compination of requested certificate type and used cipher suite is allowed. */
    /* Only certs with rsa_sign or ecdsa_sign are supported by this implementation. */
    uint8    numCertTypes;
    boolean  foundSupportedCertType = FALSE;
    uint8   *typePtr;

    /* Read the length of the list and increase the read postion variable */
    numCertTypes   = DataPtr[0];
    locReadPos    += numCertTypes + 1;
    typePtr        = &DataPtr[1];

    for(; ((numCertTypes != 0) && (foundSupportedCertType == FALSE)); numCertTypes--)  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
    {
      if(TLS_HS_CLIENT_CERT_RSA_SIGN == *typePtr)
      {
        if(TLS_KEY_EXCH_TYPE_RSA == Tls_Hs_KeyExchType[TConnId])
        {
          /* ok, supported type */
          foundSupportedCertType = TRUE;
        }
      }
      else if(TLS_HS_CLIENT_CERT_ECDSA_SIGN == *typePtr)
      {
        if(TLS_KEY_EXCH_TYPE_ECDH == Tls_Hs_KeyExchType[TConnId])
        {
          /* ok, supported type */
          foundSupportedCertType = TRUE;
        }
      }
      else
      {
        /* type not supported -> ignore this element */
      }
      typePtr = &typePtr[1];
    }
    if(foundSupportedCertType == TRUE)
    {
      /* valid/supported cert type found */
    }
    else
    {
      /* no valid or matching type offered */
      /* error */
      processingSucceeded = FALSE;
    }
  }

  /* #20 Read and ignore the list of supported signature and hash algorithms (TLS1.2 only) */
#if(TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  if( (processingSucceeded == TRUE)
#if(TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
      && (TLS_PROT_VER_TLS12 == Tls_ProtVer[TConnId])
#endif
    )
  {
    /* Currently the implementation does not support the evaluation of the received algorithms list. Since the
       implementation does not support assigning a set of multiple client certificates to one connection or to request
       a certificate from the apllication when a request has been received, evaluating the algorithm list would not be
       of much use. */
    uint16 listLen;

    /* Read the length of the list and skip it */
    IPBASE_GET_UINT16(DataPtr, locReadPos, listLen);
    locReadPos += listLen + 2;
  }
#endif

  /* #30 Read and ignore the list of accepted certificate authorities / distinguished names */
  if(processingSucceeded == TRUE)
  {
    /* Currently the implementation does not support the evaluation of the received certificate authorities list. Since
       the implementation does not support assigning a set of multiple client certificates to one connection or to
       request a certificate from the apllication when a request has been received, evaluating the list of authorities
       would not be of much use. */
    uint16 listLen;

    /* Read the length of the list and skip it */
    IPBASE_GET_UINT16(DataPtr, locReadPos, listLen);
    locReadPos += listLen + 2;
    /* #40 Set info that a client certificate is requested */
    Tls_ClientCertRequested[TConnId] = TRUE;
  }

  if(processingSucceeded == FALSE)
  {
    locReadPos = 0;  /* indicate an error */
  }

  return locReadPos;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */

/***********************************************************************************************************************
 *  Tls_VHs_RxServerFinished
 **********************************************************************************************************************/
/*! \brief      Receive handshake msg server_finished
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  DataPtr        pointer to the received (decrypted) data
 *  \return                    consumed length
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(uint16, TLS_CODE) Tls_VHs_RxServerFinished(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr )
{
  uint16 LocReadPos;
  uint8 Hash_FinshedData[TLS_HASH_MD5_LEN+TLS_HASH_SHA1_LEN];  /* this size (36byte) also fits for SHA256 (32byte) */
  uint8 Hash_LocalPrfResult[TLS_HS_X_FINISHED_LEN];

  LocReadPos = 0;

  /* finalize hash of HS messages */
  switch(Tls_HsHashAlg[TConnId])
  {
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  case TLS_HS_HASH_ALG_MD5SHA1:
    {
      /* calculate hash with MD5 and SHA1, and put both into one array */
      eslt_ErrorCode ErrorCode;

      /* MD5 finalization */
      ErrorCode = esl_finalizeMD5( &Tls_WorkSpaceMd5_Hs1[TConnId], &Hash_FinshedData[0] );
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return 0;
      }

      /* SHA1 finalization */
      ErrorCode = esl_finalizeSHA1( (eslt_WorkSpaceSHA1*)&Tls_WorkSpaceShaX_Hs1[TConnId], &Hash_FinshedData[TLS_HASH_MD5_LEN] );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return 0;
      }
      break;
    }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  case TLS_HS_HASH_ALG_SHA256:
    {
      /* calculate hash with SHA256 */
      eslt_ErrorCode ErrorCode;

      /* SHA256 finalization */
      ErrorCode = esl_finalizeSHA256( (eslt_WorkSpaceSHA256*)&Tls_WorkSpaceShaX_Hs1[TConnId], &Hash_FinshedData[0] );
      if(ESL_ERC_NO_ERROR != ErrorCode)
      {
        /* error */
        return 0;
      }
      break;
    }
#endif
  default:
    {
      /* error, inv alg */
      return 0;
    }
  }

  /* calculate the hash value (prf) inside the 'server finished' message */
  {
    uint8 *L_TgtPtr    = &Hash_LocalPrfResult[0];
    const uint8 *L_SecretPtr = &(Tls_SecParamRxActivePtr[TConnId]->master_secret[0]);
    const uint8 *L_LabelPtr  = (const uint8 *)&Tls_Hs_ServerFinishedLabel[0];
    const uint8 *L_SeedPtr   = &Hash_FinshedData[0];

    if(E_OK != Tls_VPrf( TConnId,
      L_TgtPtr, L_SecretPtr, L_LabelPtr, L_SeedPtr,
      TLS_HS_X_FINISHED_LEN, TLS_MASTER_SECRET_LEN, TLS_LABEL_SERVER_FINISHED_LEN, Tls_HsHashLen[TConnId]) )
    {
      return 0;
    }
  }

  /* compare calculated with received hash */
  {
    uint8 CompResult;
    uint8 *RecHashPtr = &DataPtr[LocReadPos];
    uint8 *LocHashPtr = &Hash_LocalPrfResult[0];
    TLS_LOG_HS_DIV(  "Tls_VHs_RxServerFinished: ------------------------------------------------------");
    CompResult = IpBase_StrCmpLen(RecHashPtr, LocHashPtr, TLS_HS_X_FINISHED_LEN);
    if(IPBASE_CMP_EQUAL == CompResult)
    {
      /* incoming hash value is correct */
      CANOE_WRITE_STRING("Tls_VHs_RxServerFinished: Hash received in -server_finished- message is correct");
    }
    else
    {
      /* error, incoming hash value is incorrect, handshake must be canceled */
      /* send alert and close connection */
      Tls_Rl_CurrentPart[TConnId] = TLS_RL_IN_HDR;
      Tls_VHs_TriggerAlertAndFin(TConnId, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_HANDSHAKE_FAILURE);
      return 0;
    }
  }

  LocReadPos += TLS_HS_X_FINISHED_LEN;

  return LocReadPos;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VPrf
 **********************************************************************************************************************/
/*! \brief      Pseudo Random Function
 *  \param[in]  TConnId        TLS connection id
 *  \param[out] TgtDataPtr     pointer where output data shall be written
 *  \param[in]  SecretPtr      pointer to secret
 *  \param[in]  LabelPtr       pointer to label
 *  \param[in]  SeedPtr        pointer to seed
 *  \param[in]  TgtLen         target data length that shall be filled
 *  \param[in]  SecretLen      length of secret
 *  \param[in]  LabelLen       length of label
 *  \param[in]  SeedLen        length of seed
 *  \return                    E_OK     if random could be generated
 *  \return                    E_NOT_OK if random could not be generated
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VPrf(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SecretPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) LabelPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SeedPtr,
    uint8 TgtLen,
    uint8 SecretLen,
    uint8 LabelLen,
    uint8 SeedLen )
{
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  uint8 P_MD5_Array[TLS_PRF_MAX_BUFFER_LEN];
  uint8 P_SHA_Array[TLS_PRF_MAX_BUFFER_LEN];
#endif
  uint8 CombinedLabel_Array[100];
  uint8 CombinedLabelLen;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  CONST(uint8, TLS_CONST) CombinedLabelLenMax = 100;

  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(TgtDataPtr)), TLS_API_ID_V_PRF, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(SecretPtr)), TLS_API_ID_V_PRF, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(LabelPtr)), TLS_API_ID_V_PRF, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(SeedPtr)), TLS_API_ID_V_PRF, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TgtDataPtr != SecretPtr), TLS_API_ID_V_PRF, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((0 != (SecretLen)), TLS_API_ID_V_PRF, TLS_E_INV_PARAM, E_NOT_OK)
  TLS_CheckDetErrorReturnValue(((TgtLen) <= (TLS_PRF_MAX_LEN)), TLS_API_ID_V_PRF, TLS_E_INV_PARAM, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((CombinedLabelLenMax > ((LabelLen) + (SeedLen))), TLS_API_ID_V_PRF, TLS_E_INV_PARAM, E_NOT_OK)
#endif
  TLS_LOG_HS_DIV_1("Tls_VPrf: function was called with target length = %d", TgtLen );

#if ((TLS_SUPPORT_PROT_VER_TLS10 == STD_OFF) || (TLS_SUPPORT_PROT_VER_TLS12 == STD_OFF))
  TLS_UNUSED_PARAMETER(TConnId)  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

  CombinedLabelLen = LabelLen + SeedLen;
  {
    uint8 i;
    for(i=0; i< LabelLen; i++)
    {
      CombinedLabel_Array[i] = LabelPtr[i];
    }
    for(i=0; i < SeedLen; i++)
    {
      CombinedLabel_Array[i+LabelLen] = SeedPtr[i];
    }
  }

#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
# if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  if(TLS_HS_HASH_ALG_MD5SHA1 == Tls_HsHashAlg[TConnId])
# endif
  {
    const uint8 *S1Ptr;
    const uint8 *S2Ptr;
    uint8 SxLen;

    /* divide secret in two halfs: */
    SxLen = (uint8)((SecretLen +1) / 2);
    S1Ptr = SecretPtr;
    S2Ptr = &SecretPtr[SecretLen - SxLen];

    /* PRF = P_MD5(S1, Label++Seed) XOR P_SHA(S2, Label++Seed);  */
    if( (E_OK == Tls_VP_Md5( &P_MD5_Array[0], S1Ptr, &CombinedLabel_Array[0], TgtLen, SxLen, CombinedLabelLen)) &&  /* PRQA S 3354 */ /* MD_TLS_9.1_3354_b */  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
        (E_OK == Tls_VP_Sha1(&P_SHA_Array[0], S2Ptr, &CombinedLabel_Array[0], TgtLen, SxLen, CombinedLabelLen)) )
    {
      /* both functions were proceeded without an error, now XOR both arrays: */
      uint8 i;
      for(i=0; i< TgtLen; i++)
      {
        TgtDataPtr[i] = P_MD5_Array[i] ^ P_SHA_Array[i];
      }
      return E_OK;
    }
    else
    {
      /* there was an error in one of the function calls */
      return E_NOT_OK;
    }
  }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
# if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  else
# endif
  {
    /* PRF = P_SHA256(S, Label++Seed);  */
    if( E_OK == Tls_VP_Sha256(&TgtDataPtr[0], SecretPtr, &CombinedLabel_Array[0], TgtLen, SecretLen, CombinedLabelLen) )
    {
      /* functions was proceeded without an error, now copy data to target buffer: */
      return E_OK;
    }
    else
    {
      /* there was an error in one of the function calls */
      return E_NOT_OK;
    }
  }
#endif

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */

#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
/***********************************************************************************************************************
 *  Tls_VP_Md5
 **********************************************************************************************************************/
/*! \brief      function generating random value using HMAC with MD5
 *  \param[in]  TgtDataPtr     pointer where output data shall be written
 *  \param[in]  SecretPtr      pointer to secret
 *  \param[in]  SeedPtr        pointer to seed
 *  \param[in]  TgtLen         target data length that shall be filled
 *  \param[in]  SecretLen      length of secret
 *  \param[in]  SeedLen        length of seed
 *  \return                    E_OK     if random could be generated
 *  \return                    E_NOT_OK if random could not be generated
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VP_Md5(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SecretPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SeedPtr,
    uint16 TgtLen,
    uint16 SecretLen,
    uint16 SeedLen )
{
  uint8 FuncRetAx[TLS_HASH_MD5_LEN];
  uint8 FuncRetAy[TLS_HASH_MD5_LEN];
  uint8 *FuncRetAActivePtr;
  uint8 *FuncRetAPassivePtr;
  uint16 FillLevel;
  Crypto_HmacMd5StoreType WorkSpaceHMacMd5;
  uint8 LastBuffer[TLS_HASH_MD5_LEN];
  uint8 *HashTargetPtr;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(TgtDataPtr)), TLS_API_ID_V_P_MD5, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(SecretPtr)), TLS_API_ID_V_P_MD5, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(SeedPtr)), TLS_API_ID_V_P_MD5, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((0 != (TgtLen)), TLS_API_ID_V_P_MD5, TLS_E_INV_PARAM, E_NOT_OK)
#endif

  FuncRetAActivePtr  = &FuncRetAx[0];
  FuncRetAPassivePtr = &FuncRetAy[0];
  FillLevel = 0;

  /* ----------------------------------------------------- */
  /* first run to calc A(1) */
  /* init, update and finalize inner hash */
  Crypto_HmacMd5Init( &WorkSpaceHMacMd5, SecretPtr, SecretLen);
  Crypto_HmacMd5Encode( &WorkSpaceHMacMd5, SeedPtr, SeedLen);
  Crypto_HmacMd5End( &WorkSpaceHMacMd5, FuncRetAActivePtr);
  /* ----------------------------------------------------- */

  while(FillLevel < TgtLen)
  {
    /* switch active and passive elements, update ALastLen and ALastPtr */
    {
      uint8 *TmpPtr;
      TmpPtr             = FuncRetAActivePtr;
      FuncRetAActivePtr  = FuncRetAPassivePtr;
      FuncRetAPassivePtr = TmpPtr;
    }

    /* ----------------------------------------------------- */
    /* use FuncRetAPassivePtr to calc outer HMAC */

    /* HMAC_MD5(secret, FuncRetAPassivePtr) */
    /* init, update and finalize outer hash */
    Crypto_HmacMd5Init( &WorkSpaceHMacMd5, SecretPtr, SecretLen);
    Crypto_HmacMd5Encode( &WorkSpaceHMacMd5, FuncRetAPassivePtr, TLS_HASH_MD5_LEN);  /* A(i-1) */
    Crypto_HmacMd5Encode( &WorkSpaceHMacMd5, SeedPtr, SeedLen);  /* seed */
    /* write hash directly into target buffer, or in temporary buffer */
    if((FillLevel +TLS_HASH_MD5_LEN) <= TgtLen)
    {
      HashTargetPtr = &TgtDataPtr[FillLevel];
      Crypto_HmacMd5End( &WorkSpaceHMacMd5, HashTargetPtr);
    }
    else
    {
      HashTargetPtr = &LastBuffer[0];
      Crypto_HmacMd5End( &WorkSpaceHMacMd5, HashTargetPtr);
      /* copy part of the data to target buffer */
      IpBase_Copy((IpBase_CopyDataType*)(&TgtDataPtr[FillLevel]), (const IpBase_CopyDataType*)(HashTargetPtr), (TgtLen - FillLevel));  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
    }

    /* ----------------------------------------------------- */
    /* init, update and finalize inner hash for next run of the loop */
    Crypto_HmacMd5Init( &WorkSpaceHMacMd5, SecretPtr, SecretLen);
    Crypto_HmacMd5Encode( &WorkSpaceHMacMd5, FuncRetAPassivePtr, TLS_HASH_MD5_LEN);  /* A(i-1) */
    Crypto_HmacMd5End( &WorkSpaceHMacMd5, FuncRetAActivePtr);

    /* ----------------------------------------------------- */
    FillLevel += TLS_HASH_MD5_LEN;

  }

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */
#endif  /* (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON) */

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
/***********************************************************************************************************************
 *  Tls_VP_Sha256
 **********************************************************************************************************************/
/*! \brief      function generating random value using HMAC with SHA256
 *  \param[in]  TgtDataPtr     pointer where output data shall be written
 *  \param[in]  SecretPtr      pointer to secret
 *  \param[in]  SeedPtr        pointer to seed
 *  \param[in]  TgtLen         target data length that shall be filled
 *  \param[in]  SecretLen      length of secret
 *  \param[in]  SeedLen        length of seed
 *  \return                    E_OK     if random could be generated
 *  \return                    E_NOT_OK if random could not be generated
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VP_Sha256(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SecretPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SeedPtr,
    uint16 TgtLen,
    uint16 SecretLen,
    uint16 SeedLen )
{
  uint8 FuncRetAx[TLS_HASH_SHA256_LEN];
  uint8 FuncRetAy[TLS_HASH_SHA256_LEN];
  uint8 *FuncRetAActivePtr;
  uint8 *FuncRetAPassivePtr;
  uint16 FillLevel;
  Crypto_HmacSha256StoreType WorkSpaceHMacSha256;
  uint8 LastBuffer[TLS_HASH_SHA256_LEN];
  uint8 *HashTargetPtr;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(TgtDataPtr)), TLS_API_ID_V_P_SHA256, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(SecretPtr)), TLS_API_ID_V_P_SHA256, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(SeedPtr)), TLS_API_ID_V_P_SHA256, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((0 != (TgtLen)), TLS_API_ID_V_P_SHA256, TLS_E_INV_PARAM, E_NOT_OK)
#endif

  FuncRetAActivePtr  = &FuncRetAx[0];
  FuncRetAPassivePtr = &FuncRetAy[0];
  FillLevel = 0;

  /* ----------------------------------------------------- */
  /* first run to calc A(1) */
  /* init, update and finalize inner hash */
  Crypto_HmacSha256Init(   &WorkSpaceHMacSha256, SecretPtr, SecretLen);
  Crypto_HmacSha256Encode( &WorkSpaceHMacSha256, SeedPtr, SeedLen);
  Crypto_HmacSha256End(    &WorkSpaceHMacSha256, FuncRetAActivePtr);
  /* ----------------------------------------------------- */

  while(FillLevel < TgtLen)
  {
    /* switch active and passive elements, update ALastLen and ALastPtr */
    {
      uint8 *TmpPtr;
      TmpPtr             = FuncRetAActivePtr;
      FuncRetAActivePtr  = FuncRetAPassivePtr;
      FuncRetAPassivePtr = TmpPtr;
    }

    /* ----------------------------------------------------- */
    /* use FuncRetAPassivePtr to calc outer HMAC */

    /* HMAC_SHA256(secret, FuncRetAPassivePtr) */
    /* init, update and finalize outer hash */
    Crypto_HmacSha256Init(   &WorkSpaceHMacSha256, SecretPtr, SecretLen);
    Crypto_HmacSha256Encode( &WorkSpaceHMacSha256, FuncRetAPassivePtr, TLS_HASH_SHA256_LEN);  /* A(i-1) */
    Crypto_HmacSha256Encode( &WorkSpaceHMacSha256, SeedPtr, SeedLen);  /* seed */
    /* write hash directly into target buffer, or in temporary buffer */
    if((FillLevel +TLS_HASH_SHA256_LEN) <= TgtLen)
    {
      HashTargetPtr = &TgtDataPtr[FillLevel];
      Crypto_HmacSha256End( &WorkSpaceHMacSha256, HashTargetPtr);
    }
    else
    {
      HashTargetPtr = &LastBuffer[0];
      Crypto_HmacSha256End( &WorkSpaceHMacSha256, HashTargetPtr);
      /* copy part of the data to target buffer */
      IpBase_Copy((IpBase_CopyDataType*)(&TgtDataPtr[FillLevel]), (const IpBase_CopyDataType*)(HashTargetPtr), (TgtLen - FillLevel));  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
    }

    /* ----------------------------------------------------- */
    /* init, update and finalize inner hash for next run of the loop */
    Crypto_HmacSha256Init(   &WorkSpaceHMacSha256, SecretPtr, SecretLen);
    Crypto_HmacSha256Encode( &WorkSpaceHMacSha256, FuncRetAPassivePtr, TLS_HASH_SHA256_LEN);  /* A(i-1) */
    Crypto_HmacSha256End(    &WorkSpaceHMacSha256, FuncRetAActivePtr);

    /* ----------------------------------------------------- */
    FillLevel += TLS_HASH_SHA256_LEN;

  }

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */
#endif  /* (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON) */

#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
/***********************************************************************************************************************
 *  Tls_VP_Sha1
 **********************************************************************************************************************/
/*! \brief      function generating random value using HMAC with SHA
 *  \param[in]  TgtDataPtr     pointer where output data shall be written
 *  \param[in]  SecretPtr      pointer to secret
 *  \param[in]  SeedPtr        pointer to seed
 *  \param[in]  TgtLen         target data length that shall be filled
 *  \param[in]  SecretLen      length of secret
 *  \param[in]  SeedLen        length of seed
 *  \return                    E_OK     if random could be generated
 *  \return                    E_NOT_OK if random could not be generated
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VP_Sha1(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtDataPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SecretPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) SeedPtr,
    uint16 TgtLen,
    uint16 SecretLen,
    uint16 SeedLen )
{
  uint8 FuncRetAx[TLS_HASH_SHA1_LEN];
  uint8 FuncRetAy[TLS_HASH_SHA1_LEN];
  uint8 *FuncRetAActivePtr;
  uint8 *FuncRetAPassivePtr;
  uint16 FillLevel;
  eslt_WorkSpaceHMACSHA1 WorkSpaceHMacSha1;
  uint8 LastBuffer[TLS_HASH_SHA1_LEN];
  uint8 *HashTargetPtr;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(TgtDataPtr)), TLS_API_ID_V_P_SHA1, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(SecretPtr)), TLS_API_ID_V_P_SHA1, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(SeedPtr)), TLS_API_ID_V_P_SHA1, TLS_E_INV_POINTER, E_NOT_OK)
  TLS_CheckDetErrorReturnValue((0 != (TgtLen)), TLS_API_ID_V_P_SHA1, TLS_E_INV_PARAM, E_NOT_OK)
#endif

  FuncRetAActivePtr  = &FuncRetAx[0];
  FuncRetAPassivePtr = &FuncRetAy[0];
  FillLevel = 0;

  (void)esl_initWorkSpaceHeader( &WorkSpaceHMacSha1.header, ESL_MAXSIZEOF_WS_HMACSHA1, NULL_PTR);

  /* ----------------------------------------------------- */
  /* first run to calc A(1) */
  /* init, update and finalize inner hash */
  (void)esl_initHashMACSHA1( &WorkSpaceHMacSha1, SecretLen, SecretPtr);
  (void)esl_updateHashMACSHA1( &WorkSpaceHMacSha1, SeedLen, SeedPtr);
  (void)esl_finalizeHashMACSHA1( &WorkSpaceHMacSha1, FuncRetAActivePtr);
  /* ----------------------------------------------------- */

  while(FillLevel < TgtLen)
  {
    /* switch active and passive elements, update ALastLen and ALastPtr */
    {
      uint8 *TmpPtr;
      TmpPtr             = FuncRetAActivePtr;
      FuncRetAActivePtr  = FuncRetAPassivePtr;
      FuncRetAPassivePtr = TmpPtr;
    }

    /* ----------------------------------------------------- */
    /* use FuncRetAPassivePtr to calc outer HMAC */

    /* HMAC_SHA1(secret, FuncRetAPassivePtr) */
    /* init, update and finalize outer hash */
    (void)esl_initHashMACSHA1( &WorkSpaceHMacSha1, SecretLen, SecretPtr);
    (void)esl_updateHashMACSHA1( &WorkSpaceHMacSha1, TLS_HASH_SHA1_LEN, FuncRetAPassivePtr);  /* A(i-1) */
    (void)esl_updateHashMACSHA1( &WorkSpaceHMacSha1, SeedLen, SeedPtr);  /* seed */
    /* write hash directly into target buffer, or in temporary buffer */
    if((FillLevel +TLS_HASH_SHA1_LEN) <= TgtLen)
    {
      HashTargetPtr = &TgtDataPtr[FillLevel];
      (void)esl_finalizeHashMACSHA1( &WorkSpaceHMacSha1, HashTargetPtr);
    }
    else
    {
      HashTargetPtr = &LastBuffer[0];
      (void)esl_finalizeHashMACSHA1( &WorkSpaceHMacSha1, HashTargetPtr);
      /* copy part of the data to target buffer */
      IpBase_Copy((IpBase_CopyDataType*)(&TgtDataPtr[FillLevel]), (const IpBase_CopyDataType*)(HashTargetPtr), (TgtLen - FillLevel));  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
    }

    /* ----------------------------------------------------- */
    /* init, update and finalize inner hash for next run of the loop */
    (void)esl_initHashMACSHA1( &WorkSpaceHMacSha1, SecretLen, SecretPtr);
    (void)esl_updateHashMACSHA1( &WorkSpaceHMacSha1, TLS_HASH_SHA1_LEN, FuncRetAPassivePtr);  /* A(i-1) */
    (void)esl_finalizeHashMACSHA1( &WorkSpaceHMacSha1, FuncRetAActivePtr);

    /* ----------------------------------------------------- */
    FillLevel += TLS_HASH_SHA1_LEN;

  }

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */
#endif  /* (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON) */

/***********************************************************************************************************************
 *  Tls_VAesEncryptData
 **********************************************************************************************************************/
/*! \brief      Encrypts data using AES encryption with 128 Bit blocksize in CBC mode
 *  \param[in]  TConnId               TLS connection index
 *  \param[in]  IVPtr                 Initialisation Vector, only needed for first data block
 *  \param[in]  PlainTextPtr          Plain text to be encrypted
 *  \param[in]  PlainTextLength       Plain text length
 *  \param[in]  CipherTextPtr         Pointer to buffer element where to store the encrypted data
 *  \param[i/o] CipherTextLengthPtr   In: Max buffer size, out: chipher text length
 *  \param[in]  Finished              TRUE if plain text includes last byte to be encrypted, padding will be added
 *  \return     TLS_E_OK              Encryption finished successfully
 *  \return     TLS_E_NOT_OK          Encryption error
 *  \return     TLS_E_SMALL_BUFFER    Target buffer to small to store the chiper text, call again with more buffer
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Tls_ReturnType, TLS_CODE) Tls_VAesEncryptData(
    uint8 TConnId,
    P2CONST(uint8,  AUTOMATIC, TLS_APPL_DATA) IVPtr,
    P2CONST(uint8,  AUTOMATIC, TLS_APPL_DATA) PlainTextPtr,
    uint16 PlainTextLength,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_DATA) CipherTextPtr,
    P2VAR(uint16, AUTOMATIC, TLS_APPL_DATA) CipherTextLengthPtr,
    boolean Finished )
{
  Tls_ReturnType result = TLS_E_OK;
  eslt_ErrorCode eslResult;
  /* tmpLen used to determin the total encrypted data size in case multiple calls to the es-lib are executed */
  uint16 tmpLen = 0;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_AES_ENCRYPT_DATA, TLS_E_INV_PARAM, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(IVPtr)), TLS_API_ID_AES_ENCRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(PlainTextPtr)), TLS_API_ID_AES_ENCRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(CipherTextPtr)), TLS_API_ID_AES_ENCRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(CipherTextLengthPtr)), TLS_API_ID_AES_ENCRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
#endif

  /* no encryption in process for this connection, init workspace with key and IV */
  if(TLS_ENC_STATE_IDLE == Tls_AESEncryptionState[TConnId])
  {
    uint8 KeyOffset;
    if(Tls_SecParamTxActivePtr[TConnId]->mac_length == TLS_HASH_SHA1_LEN)
    {
      KeyOffset = TLS_KEY_PART_OFFSET_AES128_SHA1_KEY;
    }
    else
    {
      /* sha256 */
      KeyOffset = TLS_KEY_PART_OFFSET_AES128_SHA256_KEY;
    }
    (void)esl_initEncryptAES128((eslt_WorkSpaceAES128*)&Tls_WorkSpaceAes[TConnId], (&(((uint8*)(Tls_KeyMatBlockTxPtr[TConnId]))[KeyOffset])), ESL_BM_CBC, ESL_PM_PKCS5, IVPtr);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
    Tls_AESEncryptionState[TConnId] = TLS_ENC_STATE_RUNNING;  /* Initialisation finished successfully, update encryption state for this connection */
  }

  /* workspace initialized */
  if(TLS_ENC_STATE_RUNNING == Tls_AESEncryptionState[TConnId])
  {
    /* Is there any data to be encrypted */
    if(0 != PlainTextLength)
    {
      /* store max buffer size, needed for following finalize-call */
      tmpLen = *CipherTextLengthPtr;
      /* encrypt data */
      eslResult = esl_encryptAES128((eslt_WorkSpaceAES128*)&Tls_WorkSpaceAes[TConnId], PlainTextLength, PlainTextPtr, &tmpLen, CipherTextPtr);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      /* check eror state */
      if(ESL_ERC_NO_ERROR != eslResult)
      {
        if(ESL_ERC_OUTPUT_SIZE_TOO_SHORT == eslResult)
        {
          /* more buffer required to encrypt the data */
          result = TLS_E_SMALL_BUFFER;
        }
        else
        {
          /* Encryption error */
          TLS_REPORT_ERROR_STATUS(TLS_API_ID_AES_ENCRYPT_DATA, TLS_EVENT_AES_MSG_ENCRYPTION, TLS_EVENT_STATUS_FAILED)  /* report event */
          result = TLS_E_NOT_OK;
        }
      }
    }

    /* error occured in allready executed encryption steps? */
    if(TLS_E_NOT_OK != result)
    {
      /* data encryption finished, last data byte is encrypted */
      if(TRUE == Finished)
      {
        /* set remaining buffer size */
        *CipherTextLengthPtr -= tmpLen;
        /* finalize encryption, padding will be added */
        eslResult = esl_finalizeEncryptAES128((eslt_WorkSpaceAES128*)&Tls_WorkSpaceAes[TConnId], CipherTextLengthPtr, &CipherTextPtr[tmpLen]);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
        /* check eror state */
        if(ESL_ERC_NO_ERROR != eslResult)
        {
          if(ESL_ERC_OUTPUT_SIZE_TOO_SHORT == eslResult)
          {
            /* more buffer required to encrypt the data */
            result = TLS_E_SMALL_BUFFER;
          }
          else
          {
            /* Encryption error */
            TLS_REPORT_ERROR_STATUS(TLS_API_ID_AES_ENCRYPT_DATA, TLS_EVENT_AES_MSG_ENCRYPTION, TLS_EVENT_STATUS_FAILED)  /* report event */
            result = TLS_E_NOT_OK;
          }
        }
        else
        {
          /* set cipher text length, update encryption state for this connection */
          *CipherTextLengthPtr += tmpLen;
          Tls_AESEncryptionState[TConnId] = TLS_ENC_STATE_IDLE;
        }
      }
      /* another call with more data will follow */
      else
      {
        if(NULL_PTR != CipherTextLengthPtr)  /* PRQA S 3355,3358 */ /* MD_TLS_13.7_3355_3358 */
        {
          /* set cipher text length */
          *CipherTextLengthPtr = tmpLen;
        }
      }
    }
  }
  return result;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VAesDecryptData
 **********************************************************************************************************************/
/*! \brief      Decrypts data using AES encryption with 128 Bit blocksize in CBC mode
 *  \param[in]  TConnId               TLS connection index
 *  \param[in]  IVPtr                 Initialisation Vector, only needed for first data block
 *  \param[in]  CipherTextPtr         Cipher text to be decrypted
 *  \param[in]  CipherTextLength      Cipher text length
 *  \param[in]  PlainTextPtr          Pointer to buffer element where to store the decrypted data
 *  \param[i/o] PlainTextLengthPtr    In: Max buffer size, out: plain text length
 *  \param[in]  Finished              TRUE if cipher text includes last byte to be encrypted, padding will be removed
 *  \return     TLS_E_OK              Decryption finished successfully
 *  \return     TLS_E_NOT_OK          Decryption error
 *  \return     TLS_E_SMALL_BUFFER    Target buffer to small to store the plain text, call again with more buffer
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Tls_ReturnType, TLS_CODE) Tls_VAesDecryptData(
    uint8 TConnId,
    P2CONST(uint8,  AUTOMATIC, TLS_APPL_DATA) IVPtr,
    P2CONST(uint8,  AUTOMATIC, TLS_APPL_DATA) CipherTextPtr,
    uint16 CipherTextLength,
    P2VAR(uint8,  AUTOMATIC, TLS_APPL_DATA) PlainTextPtr,
    P2VAR(uint16,  AUTOMATIC, TLS_APPL_DATA) PlainTextLengthPtr,
    boolean Finished )
{
  Tls_ReturnType result = TLS_E_OK;
  eslt_ErrorCode eslResult;
  /* tmpLen used to determin the total plain text data size in case multiple calls to the es-lib are executed */
  uint16 tmpLen = 0;
  uint16 LocDecryptedLen;

#if (TLS_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_CONNIDX(TConnId)), TLS_API_ID_AES_DECRYPT_DATA, TLS_E_INV_PARAM, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(IVPtr)), TLS_API_ID_AES_DECRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(PlainTextPtr)), TLS_API_ID_AES_DECRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(CipherTextPtr)), TLS_API_ID_AES_DECRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
  TLS_CheckDetErrorReturnValue((TLS_COND_VALID_PTR(PlainTextLengthPtr)), TLS_API_ID_AES_DECRYPT_DATA, TLS_E_INV_POINTER, TLS_E_NOT_OK)
#endif

  if(0 == *PlainTextLengthPtr)
  {
    return TLS_E_NOT_OK;  /* this function may not be called with length '0' */
  }

  /* no decryption in process for this connection, init workspace with key and IV */
  if(TLS_ENC_STATE_IDLE == Tls_AESDecryptionState[TConnId])
  {
    uint8 KeyOffset;

    if(Tls_SecParamTxActivePtr[TConnId]->mac_length == TLS_HASH_SHA1_LEN)
    {
      /* sha1 */
      KeyOffset = TLS_KEY_PART_OFFSET_AES128_SHA1_KEY;
    }
    else
    {
      /* sha256 */
      KeyOffset = TLS_KEY_PART_OFFSET_AES128_SHA256_KEY;
    }
    eslResult = esl_initDecryptAES128((eslt_WorkSpaceAES128*)&Tls_WorkSpaceAes[TConnId], (&(((uint8*)(Tls_KeyMatBlockRxActivePtr[TConnId]))[KeyOffset])), ESL_BM_CBC, ESL_PM_PKCS5, IVPtr);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
    if(ESL_ERC_NO_ERROR == eslResult)
    {
      /* Initialisation finished successfully, update decryption state for this connection */
      Tls_AESDecryptionState[TConnId] = TLS_ENC_STATE_RUNNING;
    }
    else
    {
      /* Error during workspace initialisation */
      TLS_REPORT_ERROR_STATUS(TLS_API_ID_AES_DECRYPT_DATA, TLS_EVENT_AES_MSG_DECRYPTION, TLS_EVENT_STATUS_FAILED)  /* report event */
      result = TLS_E_NOT_OK;
    }
  }

  /* workspace initialized */
  if( (TLS_E_NOT_OK != result) && (TLS_ENC_STATE_RUNNING == Tls_AESDecryptionState[TConnId]) )
  {
    /* Is there any data to be decrypted */
    if(0 != CipherTextLength)
    {
      /* store max buffer size, needed for following finalize-call */
      tmpLen = *PlainTextLengthPtr;
      /* decrypt data */
      eslResult = esl_decryptAES128((eslt_WorkSpaceAES128*)&Tls_WorkSpaceAes[TConnId], CipherTextLength, CipherTextPtr, &tmpLen, PlainTextPtr);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      /* check error state */
      if(ESL_ERC_NO_ERROR != eslResult)
      {
        if(ESL_ERC_OUTPUT_SIZE_TOO_SHORT == eslResult)
        {
          /* more buffer required to decrypt the data */
          result = TLS_E_SMALL_BUFFER;
        }
        else
        {
          /* Decryption error */
          TLS_REPORT_ERROR_STATUS(TLS_API_ID_AES_DECRYPT_DATA, TLS_EVENT_AES_MSG_DECRYPTION, TLS_EVENT_STATUS_FAILED)  /* report event */
          result = TLS_E_NOT_OK;
        }
      }
    }

    /* error occured in allready executed decryption steps? */
    if(TLS_E_NOT_OK != result)
    {
      /* data decryption finished, last data byte is decrypted */
      if(TRUE == Finished)
      {
        uint8 DummyByte = 0;
        uint8 *LocPlainTextPtr;

        /* set remaining buffer size */
        *PlainTextLengthPtr -= tmpLen;

        LocDecryptedLen = tmpLen;
        LocPlainTextPtr = &PlainTextPtr[tmpLen];
        tmpLen = *PlainTextLengthPtr;
        /* finalize decryption:
           - decode one dummy byte so that the cryptovision library decodes the last block
           - remove the padding manually */
        eslResult = esl_decryptAES128((eslt_WorkSpaceAES128*)&Tls_WorkSpaceAes[TConnId], 1, &DummyByte, &tmpLen, LocPlainTextPtr);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
        /* check eror state */
        if(ESL_ERC_NO_ERROR != eslResult)
        {
          if(ESL_ERC_OUTPUT_SIZE_TOO_SHORT == eslResult)
          {
            /* more buffer required to decrypt the data */
            result = TLS_E_SMALL_BUFFER;
          }
          else
          {
            /* Decryption error */
            TLS_REPORT_ERROR_STATUS(TLS_API_ID_AES_DECRYPT_DATA, TLS_EVENT_AES_MSG_DECRYPTION, TLS_EVENT_STATUS_FAILED)  /* report event */
            result = TLS_E_NOT_OK;
          }
        }
        else
        {
          /* set plain text length, update decryption state for this connection */
          LocDecryptedLen += tmpLen;
          Tls_AESDecryptionState[TConnId] = TLS_ENC_STATE_IDLE;

          /* Check the AES padding for being valid. All padding bytes have to have the same value. */
          {
            uint8 PaddingLen = PlainTextPtr[LocDecryptedLen -1];  /* There won't be a wraparound, LocDecryptedLen is always bigger than '0'. */  /* PRQA S 3372 */ /* MD_TLS_21.1_3372_a */
            uint16_least PaddingIndex;
            uint16_least PaddingIndexMin;
            if(PaddingLen > LocDecryptedLen)  /* check the last byte value for being small than the total length */
            {
              result = TLS_E_NOT_OK;
            }

            PaddingIndex    = LocDecryptedLen -1;
            PaddingIndexMin = LocDecryptedLen - (PaddingLen +1);
            while(PaddingIndex > PaddingIndexMin)  /* check bytes for being all equal */
            {
              PaddingIndex--;
              if(PlainTextPtr[PaddingIndex] != PaddingLen)
              {
                /* error, padding byte does not equal the padding length byte */
                result = TLS_E_NOT_OK;
                break;  /* leave the while-loop */
              }
            }
          }

          /* remove AES padding (read padding length in last decrypted byte) */
          if(result == TLS_E_OK)
          {
            *PlainTextLengthPtr = (LocDecryptedLen - PlainTextPtr[LocDecryptedLen -1]);  /* There won't be a wraparound! Only the padding is removed. */  /* PRQA S 3372 */ /* MD_TLS_21.1_3372_a */
          }
          else
          {
            /* do nothing, error will already be handled */
          }
        }
      }
      /* another call with more data will follow */
      else
      {
        if(NULL_PTR != PlainTextLengthPtr)  /* PRQA S 3355,3358 */ /* MD_TLS_13.7_3355_3358 */
        {
          /* set plain text length */
          *PlainTextLengthPtr = tmpLen;
        }
      }
    }
  }
  return result;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6060 */ /* MD_TCPIP_TCP_STPAR_6060 */  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Tls_VInitConnVars
 **********************************************************************************************************************/
/*! \brief      init connection depending variables
 *  \param[in]  TConnId        TLS connection id
 *  \return                    void
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VInitConnVars(
    uint8 TConnId )
{
  Tls_ConnState[TConnId]               = TLS_STATE_TCP_CLOSED_TLS_CLOSED;
  TLS_LOG_DIV_1("Tls_VInitConnVars: connection state changed to %d", Tls_ConnState[TConnId]);

  Tls_Rl_HdrReadPos[TConnId]           = 0;
  Tls_PendingAlertId[TConnId]          = TLS_ALERT_DESC_NONE;
  Tls_HandShakeState[TConnId]          = TLS_HS_STATE_INIT;
  Tls_Rl_CurrentPart[TConnId]          = TLS_RL_IN_HDR;
  Tls_TxConfRemaining[TConnId]         = 0;
  Tls_TxBufRequested[TConnId]          = 0;
  Tls_RxPlainTextFillLevel[TConnId]    = 0;
  Tls_ClientCertRequested[TConnId]     = FALSE;
  Tls_AppDataLenWaitingForRec[TConnId] = 0;

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  Tls_ProtVer[TConnId] = TLS_PROT_VER_TLS12;
#else
  Tls_ProtVer[TConnId] = TLS_PROT_VER_TLS10;
#endif

  /* (re-)set security parameters */
  Tls_SecParamTxActivePtr[TConnId]     = &Tls_SecParam[(TConnId *4) + 0];
  Tls_SecParamTxPassivePtr[TConnId]    = &Tls_SecParam[(TConnId *4) + 1];
  Tls_SecParamRxActivePtr[TConnId]     = &Tls_SecParam[(TConnId *4) + 2];
  Tls_SecParamRxPassivePtr[TConnId]    = &Tls_SecParam[(TConnId *4) + 3];

  Tls_KeyMatBlockTxPtr[TConnId]        = &Tls_KeyMatBlocks[(TConnId *3) + 0];
  Tls_KeyMatBlockRxActivePtr[TConnId]  = &Tls_KeyMatBlocks[(TConnId *3) + 1];
  Tls_KeyMatBlockRxPassivePtr[TConnId] = &Tls_KeyMatBlocks[(TConnId *3) + 2];

  /* security parameters */
  /* TX: */
  Tls_SecParamTxActivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_NULL;
  Tls_SecParamTxActivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_STREAM;
  Tls_SecParamTxActivePtr[TConnId]->cipher_suite          = TLS_NULL_WITH_NULL_NULL;
  Tls_SecParamTxActivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_NULL;
  Tls_SecParamTxActivePtr[TConnId]->mac_length            = TLS_HASH_NULL_LEN;
  /* RX: */
  Tls_SecParamRxActivePtr[TConnId]->bulk_cipher_algorithm = TLS_BULK_CIPHER_ALGORITHM_NULL;
  Tls_SecParamRxActivePtr[TConnId]->cipher_type           = TLS_CIPHER_TYPE_STREAM;
  Tls_SecParamRxActivePtr[TConnId]->cipher_suite          = TLS_NULL_WITH_NULL_NULL;
  Tls_SecParamRxActivePtr[TConnId]->mac_algorithm         = TLS_MAC_ALGORITHM_NULL;
  Tls_SecParamRxActivePtr[TConnId]->mac_length            = TLS_HASH_NULL_LEN;

  Tls_TxCipherTextSizes[TConnId]      = 0;  /* no transmission before handshake is finished */
  Tls_TxPlainTextSizes[TConnId]       = 0;  /* no transmission before handshake is finished */

  Tls_VSeqNumInit(&Tls_TxSeqNum[TConnId]);
  Tls_VSeqNumInit(&Tls_RxSeqNum[TConnId]);

  Tls_AESEncryptionState[TConnId]     = TLS_ENC_STATE_IDLE;
  Tls_AESDecryptionState[TConnId]     = TLS_ENC_STATE_IDLE;

  Tls_ConnNvmRequStatus[TConnId]      = TLS_NVM_REQU_IDLE;
  Tls_ConnNvmRequBlockId[TConnId]     = TLS_NVM_INV_BLOCK_ID;
  Tls_RxPbufList[TConnId *2     ].len = 0;
  Tls_RxPbufList[(TConnId *2) +1].len = 0;
  Tls_RxBlockingState[TConnId]        = TLS_RX_BLOCKING_STATE_NONE;

  Tls_ClientCertPtr[TConnId]          = NULL_PTR;
  Tls_ClientKeyPtr[TConnId]           = NULL_PTR;
  Tls_ClientCertLen[TConnId]          = 0;
  Tls_ClientKeyLen[TConnId]           = 0;

#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  Tls_RlRxIvFillLevel[TConnId]        = 0;
#if (TLS_SUPPORT_AES_GCM == STD_ON)
  Tls_GcmRxTagFieldFillLevel[TConnId] = 0;
#endif
#endif

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
  Tls_HeartbeatModeOwn[TConnId]       = TLS_HB_MODE_NONE;  /* default has to be 'none' according to RFC 6520 */
  Tls_HeartbeatModePeer[TConnId]      = TLS_HB_MODE_NONE;  /* init value for peer mode */
  Tls_HeartbeatStatus[TConnId]        = TLS_HEARTBEAT_STATUS_DISABLED;
  Tls_HeartbeatIdlePeriodS[TConnId]   = TLS_CFG_HEARTBEAT_PERIOD_DEFAULT_S;
  Tls_HeartbeatIdleCountdown[TConnId] = 0;
  Tls_HeartbeatCounter[TConnId]       = 0;
#endif

  {
    /* init AES128 WS header */
    (void)esl_initWorkSpaceHeader(&Tls_WorkSpaceAes[TConnId].header, ESL_MAXSIZEOF_WS_AES128, NULL_PTR);  /* PRQA S 3393,3395 */ /* MD_TLS_12.1_3393_3395 */

#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
    /* init MD5 WS header */
    (void)esl_initWorkSpaceHeader(&Tls_WorkSpaceMd5_Hs1[TConnId].header, ESL_MAXSIZEOF_WS_MD5, NULL_PTR);
# if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    (void)esl_initWorkSpaceHeader(&Tls_WorkSpaceMd5_Hs2[TConnId].header, ESL_MAXSIZEOF_WS_MD5, NULL_PTR);
# endif
#endif
    /* init SHAx WS header */
    (void)esl_initWorkSpaceHeader(&Tls_WorkSpaceShaX_Hs1[TConnId].header, ESL_MAXSIZEOF_WS_SHA256, NULL_PTR);
    /* Tls_WorkSpaceShaX_Hs2 does not have to be initialized if only TLS1.0 or TLS1.2 is supported since an other workspace will be copied into this one. */
#if ((TLS_SUPPORT_PROT_VER_TLS10 == STD_ON) && (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON))
    (void)esl_initWorkSpaceHeader(&Tls_WorkSpaceShaX_Hs2[TConnId].header, ESL_MAXSIZEOF_WS_SHA256, NULL_PTR);
#endif
  }

#if (TLS_SUPPORT_OCSP == STD_ON)
  Tls_OcspWorkingMode[TConnId]          = TLS_OCSP_MODE_SUPPORT_V1;
#endif

  if ( 0 != Tls_RxRemainingAckLen[TConnId] )
  {
    Tls_VReleaseBufSegment(TConnId, &Tls_RxBufSegMgt[TConnId], Tls_RxRemainingAckLen[TConnId]);
    Tls_RxRemainingAckLen[TConnId] = 0;
  }

  /* reset the conn 2 socket mapping */
  Tls_SockHnd2TlsConnMap[Tls_CfgTlsConn2SockHndMap[TConnId]] = TLS_INV_CONN_ID;
  Tls_CfgTlsConn2SockHndMap[TConnId] = TCPIP_INV_SOCK_IDX;

  Tls_VInitBufSegment(&Tls_RxBufSegMgt[TConnId]);

#if (TLS_SUPPORT_CRL == STD_ON)
  Tls_CrlValidationState[TConnId] = TLS_CRL_VALIDATION_IDLE;
#endif

  TLS_LOG_DIV("Tls_VInitConnVars: Connection is reset and prepared for new opening");

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VRl_CheckRxMac
 **********************************************************************************************************************/
/*! \brief      check MAC of received message
 *  \param[in]  TConnId        TLS connection id
 *  \return     E_OK           MAC is correct
 *  \return     E_NOT_OK       MAC is not correct
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VRl_CheckRxMac(
    uint8 TConnId )
{
  /* calculate MAC of the data part of the received message and compare it to the received MAC value */
  uint8 *MacRecPtr;
  uint8 *MacLocPtr;
  uint32 SeqNum[2];
  uint8 CompResult;
  uint8 Buf[5];
  uint8 KeyPartMacOffs;
  uint8 LocMacArray[TLS_HASH_SHA256_LEN];  /* supported HMAC sizes up to 32 bytes (sha256) */

  MacRecPtr = &(Tls_GetRxPlainTextStartPtr(TConnId))[Tls_RxPlainTextFillLevel[TConnId]];
  MacLocPtr = &LocMacArray[0];

  /* The offset of the MAC bytes inside the key material is allways 0, independent from the used cipher suite */
  KeyPartMacOffs = 0;

  /* fill in sequence number */
  SeqNum[0] = IPBASE_HTON32(Tls_RxSeqNum[TConnId].UpperVal);
  SeqNum[1] = IPBASE_HTON32(Tls_RxSeqNum[TConnId].LowerVal);

  {
    /* fill in version info and length */
    /* content type: (1 byte) (handshake, ccs, alert, appl data) */
    Buf[0] = Tls_RxProtocol[TConnId];
    /* protocol version: (2 byte) (TLS 1.2 (= SSL 3.3)) */
    Buf[1] = TLS_MAJOR_VER;
    switch(Tls_ProtVer[TConnId])
    {
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
    case TLS_PROT_VER_TLS12:
      {
        Buf[2] = TLS_MINOR_VER_TLS12;
        break;
      }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
    case TLS_PROT_VER_TLS10:
      {
        Buf[2] = TLS_MINOR_VER_TLS10;
        break;
      }
#endif
    default:
      {
        /* one of the above elements will always be true -> do nothing here */
      }
    }

    /* fragment length: (2 byte) */
    Buf[3] = (uint8)(((Tls_Rl_DecryptedDataLen[TConnId]) >> 8) & 0xFF);
    Buf[4] = (uint8)(((Tls_Rl_DecryptedDataLen[TConnId]) >> 0) & 0xFF);
  }

  /* now calc MAC, depending on MAC algortihm -------------------- */
  if(TLS_MAC_ALGORITHM_HMAC_SHA1 == Tls_SecParamRxActivePtr[TConnId]->mac_algorithm)
  {
    eslt_WorkSpaceHMACSHA1 LocWorkSpaceSha1;

    /* init HMAC_SHA1 workspace header, init HMAC_SHA1 workspace, update HMAC_SHA1 with data, and finalize HMAC_SHA1 */
    if( (ESL_ERC_NO_ERROR != esl_initWorkSpaceHeader(&LocWorkSpaceSha1.header, ESL_MAXSIZEOF_WS_HMACSHA1, NULL_PTR)) ||  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
        (ESL_ERC_NO_ERROR != esl_initHashMACSHA1( &LocWorkSpaceSha1, TLS_HASH_SHA1_LEN, (&(((uint8*)(Tls_KeyMatBlockRxActivePtr[TConnId]))[KeyPartMacOffs])) )) ||  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
        (ESL_ERC_NO_ERROR != esl_updateHashMACSHA1( &LocWorkSpaceSha1, 8, (const eslt_Byte*)&SeqNum[0] )) ||  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_x */
        (ESL_ERC_NO_ERROR != esl_updateHashMACSHA1( &LocWorkSpaceSha1, 5, &Buf[0] )) ||
        (ESL_ERC_NO_ERROR != esl_updateHashMACSHA1( &LocWorkSpaceSha1, Tls_Rl_DecryptedDataLen[TConnId], &((Tls_GetRxPlainTextStartPtr(TConnId))[Tls_RxPlainTextFillLevel[TConnId] - Tls_Rl_DecryptedDataLen[TConnId]]) )) ||
        (ESL_ERC_NO_ERROR != esl_finalizeHashMACSHA1( &LocWorkSpaceSha1, MacLocPtr )) )
    {
      /* error */
      return E_NOT_OK;
    }

    /* now compare received mac with calculated MAC */
    CompResult = IpBase_StrCmpLen(MacRecPtr, MacLocPtr, TLS_HASH_SHA1_LEN);
  }
  else if(TLS_MAC_ALGORITHM_HMAC_SHA256 == Tls_SecParamRxActivePtr[TConnId]->mac_algorithm)
  {
    Crypto_HmacSha256StoreType LocWsHmacSha256;

    /* init HMAC_SHA256 workspace */
    Crypto_HmacSha256Init( &LocWsHmacSha256, (&(((uint8*)(Tls_KeyMatBlockRxActivePtr[TConnId]))[KeyPartMacOffs])), TLS_HASH_SHA256_LEN );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_keymat */
    /* update HMAC_SHA256 with data */
    Crypto_HmacSha256Encode( &LocWsHmacSha256, (const uint8*)&SeqNum[0], 8 );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_x */
    Crypto_HmacSha256Encode( &LocWsHmacSha256, &Buf[0], 5 );
    Crypto_HmacSha256Encode( &LocWsHmacSha256, &((Tls_GetRxPlainTextStartPtr(TConnId))[Tls_RxPlainTextFillLevel[TConnId] - Tls_Rl_DecryptedDataLen[TConnId]]), Tls_Rl_DecryptedDataLen[TConnId] );
    /* finalize HMAC_SHA256 */
    Crypto_HmacSha256End( &LocWsHmacSha256, MacLocPtr );

    /* now compare received mac with calculated MAC */
    CompResult = IpBase_StrCmpLen(MacRecPtr, MacLocPtr, TLS_HASH_SHA256_LEN);
  }
  else
  {
    /* error, but this should never happen since cipher suite is checked, earlier. */
    /* do nothing */
    return E_NOT_OK;
  }

  /* evaluate comparism result */
  if(IPBASE_CMP_EQUAL == CompResult)
  {
    /* OK */
    TLS_LOG_RL(  "Tls_VRl_CheckRxMac: MAC of received message is correct");

    Tls_VSeqNumInc(&Tls_RxSeqNum[TConnId]);  /* inc sequence number */
    return E_OK;
  }
  else
  {
    /* error, incoming hash value is incorrect */
    TLS_LOG_RL(  "Tls_VRl_CheckRxMac: MAC of received message is NOT correct --- ERROR !!!");
  }

  return E_NOT_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  Tls_VCalcKeyBlock
 **********************************************************************************************************************/
/*! \brief      calculate key block
 *  \param[in]  TConnId        TLS connection id
 *  \param[in]  TgtPtr         pointer where data shall be written to
 *  \param[in]  TgtSize        length of the target array
 *  \return                    void
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VCalcKeyBlock(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) TgtPtr,
    uint8 TgtSize )
{
  Std_ReturnType RetValue;
  uint8 Seed[2* TLS_HS_RANDOM_LEN];
  Tls_SecurityParametersType *SecParPtr;

  /*
  RFC:
  key_block = PRF( SecurityParameters.master_secret,
                   "key expansion",
                   SecurityParameters.server_random + SecurityParameters.client_random );
  */

  /* work on passive security parameters */
  SecParPtr = Tls_SecParamTxPassivePtr[TConnId];

  /* copy server and client random into one array */
  IpBase_Copy((IpBase_CopyDataType*)(&Seed[                0]), (const IpBase_CopyDataType*)(&SecParPtr->server_random[0]), TLS_HS_RANDOM_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
  IpBase_Copy((IpBase_CopyDataType*)(&Seed[TLS_HS_RANDOM_LEN]), (const IpBase_CopyDataType*)(&SecParPtr->client_random[0]), TLS_HS_RANDOM_LEN);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */

  /* now calculate key block content */
  RetValue = Tls_VPrf( TConnId,
                       TgtPtr,
                       &(SecParPtr->master_secret[0]),
                       (const uint8*)&Tls_Hs_KeyExpansionLabel[0],
                       &Seed[0],
                       TgtSize,  /* TLS_KEY_BLOCK_SIZE_XX, */
                       TLS_MASTER_SECRET_LEN,
                       TLS_LABEL_KEY_EXPENSION_LEN,
                       (2* TLS_HS_RANDOM_LEN) );

  return RetValue;
}

/***********************************************************************************************************************
 *  Tls_VSeqNumInit
 **********************************************************************************************************************/
/*! \brief      initialize sequence numbers for MAC calculation
 *  \param[in]  SeqNumPtr      pointer to sequence number
 *  \return                    void
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VSeqNumInit(
    P2VAR(Tls_SeqNumType, AUTOMATIC, TLS_APPL_DATA) SeqNumPtr )
{
  SeqNumPtr->LowerVal = 0;
  SeqNumPtr->UpperVal = 0;
}

/***********************************************************************************************************************
 *  Tls_VSeqNumInc
 **********************************************************************************************************************/
/*! \brief      increase sequence number
 *  \param[in]  SeqNumPtr      pointer to sequence number
 *  \return                    void
 *  \context    interrupt or task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VSeqNumInc(
    P2VAR(Tls_SeqNumType, AUTOMATIC, TLS_APPL_DATA) SeqNumPtr )
{
  if(SeqNumPtr->LowerVal == 0xFFFFFFFFu)
  {
    SeqNumPtr->LowerVal = 0;
    /* inc upper value */
    SeqNumPtr->UpperVal++;
  }
  else
  {
    SeqNumPtr->LowerVal++;
  }
}

/***********************************************************************************************************************
 *  Tls_VReadCertTimeInfo
 **********************************************************************************************************************/
/*! \brief      Read time and date (from certificate) and convert it into a different format
 *  \param[in]  DataPtr        pointer to the time sequence inside the certificate
 *  \param[in]  Len            length of the time sequence
 *  \param[out] Time32Ptr      pointer where the converted time shall be stored
 *  \return                    void
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VReadCertTimeInfo(
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint8 Len,
    CONSTP2VAR(uint32, AUTOMATIC, TLS_CONST) Time32Ptr )
{
  Tls_TimeDTType TimeDT;
  uint32         TempInt;
  uint8          TempOffset;

  /* copy date and time from certificate: */

  if(13 == Len)
  {
    /* UTCTime */
    (void)IpBase_ConvString2Int(&DataPtr[0], 2, &TempInt);  /* cert.year starts in 1950 */
    if(TempInt < 50)
    {
      /* years 2000 .. 2049 */
      TimeDT.Year = (uint8)TempInt + 100;
    }
    else
    {
      /* years 1950 .. 1999 */
      /* nothing to do */
    }
    TempOffset = 2;
  }
  else
  {
    /* (15 == Len) GeneralizedTime */
    (void)IpBase_ConvString2Int(&DataPtr[0], 4, &TempInt);
    /* 4 digit year */
    TimeDT.Year = (uint8)(TempInt - 1900);  /* This will fail starting in the year 2156 */
    TempOffset = 4;
  }

  (void)IpBase_ConvString2Int(&DataPtr[TempOffset +0], 2, &TempInt);  /* cert.mon = 1..12 */
  TimeDT.Mon  = (uint8)TempInt - 1;

  (void)IpBase_ConvString2Int(&DataPtr[TempOffset +2], 2, &TempInt);  /* cert.mday = 1..31 */
  TimeDT.Mday = (uint8)TempInt;

  (void)IpBase_ConvString2Int(&DataPtr[TempOffset +4], 2, &TempInt);
  TimeDT.Hour = (uint8)TempInt;

  (void)IpBase_ConvString2Int(&DataPtr[TempOffset +6], 2, &TempInt);
  TimeDT.Min  = (uint8)TempInt;

  (void)IpBase_ConvString2Int(&DataPtr[TempOffset +8], 2, &TempInt);
  TimeDT.Sec  = (uint8)TempInt;

  Tls_VConvertTimeDT2S(&TimeDT, Time32Ptr);  /* Time32Ptr overrun in the year 2106 */

  TLS_LOG_HS_DIV_8("Tls_VReadCertTimeInfo: year %d (from 1900), month %d, day %d, hour %d, min %d, sec %d, invalid %d %d", \
    TimeDT.Year, TimeDT.Mon, TimeDT.Mday, TimeDT.Hour, TimeDT.Min, TimeDT.Sec, 0, 0);
  TLS_LOG_HS_DIV_1("Tls_VReadCertTimeInfo: seconds since 1970: 0x%x", *Time32Ptr);
}

/***********************************************************************************************************************
 *  Tls_VParseServerCert
 **********************************************************************************************************************/
/*! \brief      parse server cert that was read from NVM
 *  \param[out] CertDesc       pointer where the certificate information shall be stored
 *  \param[in]  HashInfo       information what shall be hashed, the cert issuer or subject
 *  \param[in]  DataPtr        pointer to the certificate
 *  \param[in]  DataLen        length of the block that contains the certificate
 *  \return     E_OK           certificate could be parsed
 *  \return     E_NOT_OK       certificate could not be parsed
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VParseServerCert(
    CONSTP2VAR(Tls_Cert_Descr, AUTOMATIC, TLS_CONST) CertDesc,
    uint8 HashInfo,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen )
{
  uint32 ObjLen;
  uint32 ObjOffs;
  uint8  CertVer;  /* '1' or '2' (2 and 3 have the same structure) */
  uint8  PubKeyType;  /* 1=RSA, 2=ECC */
  uint32 LocDataLen = DataLen;  /* Length of the whole certificate. Value initialized with the length of the NVM block */

  CertDesc->CertFoundElements = 0;

  /* Check the input data for being reasonable. */
  if(DataPtr[0] != TLS_BER_TYPE_CONSTRUCTED_SEQUENCE)
  {
    /* Invalid input. The content needs to be a certificate, and a certificate always starts with 0x30 (BER coding type
       'set'). Mainly this check shall filter out empty NVM blocks. */
    return E_NOT_OK;
  }

  /* now parse the certificate */
  {
    CONST(uint8, TLS_CONST) EleNrCertRoot[1]      = { 1 };           /* chapter 1: root of the whole cert */
    CONST(uint8, TLS_CONST) EleNrSignedCert[2]    = { 1, 1 };        /* chapter 1.1: signed cert */
    CONST(uint8, TLS_CONST) EleNrCertSignAlg[2]   = { 1, 2 };        /* chapter 1.2: cert signature algorithm */
    CONST(uint8, TLS_CONST) EleNrCertSign[2]      = { 1, 3 };        /* chapter 1.3: cert signature */

    CONST(uint8, TLS_CONST) CertTable[2][8][TLS_HS_CERT_PARSING_MAX_ELE_DEPTH] =  /* '0'-values are invalid */ /* TLS_HS_CERT_PARSING_MAX_ELE_DEPTH is 5 */
    {
      {
        /* v1 */
        { 1, 1, 1, 0, 0 },  /* chapter 1.1.1: cert serial number */
        { 1, 1, 3, 0, 0 },  /* chapter 1.1.3: cert issuer */
        { 1, 1, 4, 1, 0 },  /* chapter 1.1.4.1: validity start */
        { 1, 1, 4, 2, 0 },  /* chapter 1.1.4.2: validity end */
        { 1, 1, 5, 0, 0 },  /* chapter 1.1.5: cert subject */
        { 1, 1, 6, 1, 1 },  /* chapter 1.1.6.1.1: cert pub key type */
        { 1, 1, 6, 1, 2 },  /* chapter 1.1.6.1.2: cert ecc curve oid */
        { 1, 1, 6, 2, 0 }   /* chapter 1.1.6.2: cert pub key-mat */
      },
      {
        /* v3 */
        { 1, 1, 2, 0, 0 },  /* chapter 1.1.2: cert serial number */
        { 1, 1, 4, 0, 0 },  /* chapter 1.1.4: cert issuer */
        { 1, 1, 5, 1, 0 },  /* chapter 1.1.5.1: validity start */
        { 1, 1, 5, 2, 0 },  /* chapter 1.1.5.2: validity end */
        { 1, 1, 6, 0, 0 },  /* chapter 1.1.6: cert subject */
        { 1, 1, 7, 1, 1 },  /* chapter 1.1.7.1.1: cert pub key type */
        { 1, 1, 7, 1, 2 },  /* chapter 1.1.7.1.2: cert ecc curve oid */
        { 1, 1, 7, 2, 0 }   /* chapter 1.1.7.2: cert pub key-mat */
      }
    };
    CONST(uint8, TLS_CONST) CertTableElementDepth[8] =
    {
      3,  /* chapter : cert serial number */
      3,  /* chapter : cert issuer */
      4,  /* chapter : validity start */
      4,  /* chapter : validity end */
      3,  /* chapter : cert subject */
      5,  /* chapter : cert pub key type */
      5,  /* chapter : cert ecc curve oid */
      4   /* chapter : cert pub key-mat */
    };
    const uint8 *ElePtr;

    /* sub-chapters: */
    CONST(uint8, TLS_CONST) EleNrCertRsaPubKey[2]    = { 1, 1 }; /* chapter 1.1: rsa public key */
    CONST(uint8, TLS_CONST) EleNrCertRsaPubExp[2]    = { 1, 2 }; /* chapter 1.2: rsa public exponent */
    uint8_least             CertTableElementIndex;

    IpBase_ReturnType           Res;
    IpBase_BerWorkspaceType     Workspace;
    IpBase_BerStackElementType  BerStack[TLS_HS_CERT_PARSING_MAX_ELE_DEPTH];  /* max depth of one element */
    IpBase_BerElementType       EleDesc;
    IpBase_BerElementType       EleDescKeyMat;

    /* Init the BER decoding workspace once */
    IpBase_BerInitWorkspace(&Workspace, &BerStack[0], TLS_HS_CERT_PARSING_MAX_ELE_DEPTH);

    /* # Find the position of the certificate's first chapter */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrCertRoot[0], TLS_HS_CERT_PARSING_DEPTH_ONE, DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* now, EleDesc contains the index information */
      /* chapter one */
      LocDataLen = EleDesc.EndIdx - EleDesc.Idx;
      CertDesc->CertCh1.ContOffs = (uint16)EleDesc.ContentIdx;
      CertDesc->CertCh1.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_CERT_CH1;
    }
    else
    {
      return E_NOT_OK;
    }

    /* # Find the position of the certificate's 'signed certificate' chapter */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrSignedCert[0], TLS_HS_CERT_PARSING_DEPTH_TWO, DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* signed cert */
      CertDesc->SignedCert.ContOffs = (uint16)EleDesc.ContentIdx;
      CertDesc->SignedCert.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_SIGNED_CERT;

      /* # Read the first byte of the signed certificate chapter to find out wether the next sub-chapter is the serial
         number or the version information. */
      /* The first byte descibes the class of the next BER-encoded chapter: either it is 0x02 (integer value) and
         therefor the serial number, or it is 0xA0 (optional element) and therefore the certificate version field */
      if(0x02 == DataPtr[EleDesc.ContentIdx])
      {
        /* v1 certificate: first element is the serial number, there is no version number  */
        CertVer = 1;
      }
      else
      {
        /* v2 or v3 certificate: first element is the version number */
        CertVer = 2;
      }
    }
    else
    {
      return E_NOT_OK;
    }

#if (TLS_SUPPORT_CRL)
    /* # Find the position of the certificate serial number */
    /* this element is only needed for CRL usage */
    CertTableElementIndex = 0;
    ElePtr = &CertTable[CertVer-1][CertTableElementIndex][0];
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, ElePtr, CertTableElementDepth[CertTableElementIndex], DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* cert serial number */
      CertDesc->SerialNumber.ContOffs = (uint16)EleDesc.ContentIdx;
      CertDesc->SerialNumber.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_SERIAL_NUMBER;
    }
    else
    {
      return E_NOT_OK;
    }
#endif

    /* # Find the position of the certificate issuer */
    CertTableElementIndex = 1;
    ElePtr = &CertTable[CertVer-1][CertTableElementIndex][0];
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, ElePtr, CertTableElementDepth[CertTableElementIndex], DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* cert issuer */
      CertDesc->Issuer.ContOffs = (uint16)EleDesc.ContentIdx;
      CertDesc->Issuer.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_ISSUER;
    }
    else
    {
      return E_NOT_OK;
    }

    /* # Find the position of the certificate validity time 'not before' */
    CertTableElementIndex = 2;
    ElePtr = &CertTable[CertVer-1][CertTableElementIndex][0];
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, ElePtr, CertTableElementDepth[CertTableElementIndex], DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* validity start */
      CertDesc->ValidNotBefore.ContOffs = (uint16)EleDesc.ContentIdx;
      CertDesc->ValidNotBefore.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_VALID_NOT_BEFORE;
    }
    else
    {
      return E_NOT_OK;
    }

    /* # Find the position of the certificate validity time 'not after' */
    CertTableElementIndex = 3;
    ElePtr = &CertTable[CertVer-1][CertTableElementIndex][0];
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, ElePtr, CertTableElementDepth[CertTableElementIndex], DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* validity end */
      CertDesc->ValidNotAfter.ContOffs = (uint16)EleDesc.ContentIdx;
      CertDesc->ValidNotAfter.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_VALID_NOT_AFTER;
    }
    else
    {
      return E_NOT_OK;
    }

    /* # Find the position of the certificate subject */
    CertTableElementIndex = 4;
    ElePtr = &CertTable[CertVer-1][CertTableElementIndex][0];
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, ElePtr, CertTableElementDepth[CertTableElementIndex], DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* cert subject */
      CertDesc->Subject.ContOffs = (uint16)EleDesc.ContentIdx;
      CertDesc->Subject.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_SUBJECT;
    }
    else
    {
      return E_NOT_OK;
    }

    /* # Find the position of the certificate key type */
    CertTableElementIndex = 5;
    ElePtr = &CertTable[CertVer-1][CertTableElementIndex][0];
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, ElePtr, CertTableElementDepth[CertTableElementIndex], DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      uint8_least KeyAlg;
      /* cert key type */

      CertDesc->KeyAlgId.ContOffs = (uint16)EleDesc.ContentIdx;
      CertDesc->KeyAlgId.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_KEY_ALG_ID;

      /* Evaluate the key identifier */
      KeyAlg = Tls_VReadKeyAlgId(&DataPtr[EleDesc.ContentIdx], CertDesc->KeyAlgId.ContLen);
      if(KeyAlg == TLS_KEY_ALG_TYPE_ECC)
      {
        PubKeyType = TLS_KEY_EXCH_TYPE_ECDH;
      }
      else if(KeyAlg == TLS_KEY_ALG_TYPE_RSA)
      {
        PubKeyType = TLS_KEY_EXCH_TYPE_RSA;
      }
      else
      {
        return E_NOT_OK;
      }
    }
    else
    {
      return E_NOT_OK;
    }

    /* # Find the position of the ECC curve OID (object id) */
    if(TLS_KEY_EXCH_TYPE_ECDH == PubKeyType)
    {
      /* cert ecc curve oid */
      CertTableElementIndex = 6;
      ElePtr = &CertTable[CertVer-1][CertTableElementIndex][0];
      Res = IpBase_BerGetElement(&Workspace, &EleDesc, ElePtr, CertTableElementDepth[CertTableElementIndex], DataPtr, LocDataLen);
      if( IPBASE_E_OK == Res )
      {
        CertDesc->EccCurveId.ContOffs = (uint16)EleDesc.ContentIdx;
        CertDesc->EccCurveId.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
        CertDesc->CertFoundElements |= TLS_CERT_ELE_KEY_ECC_CURVE;
      }
      else
      {
        return E_NOT_OK;
      }
    }

    /* # Find the position of the public key material */
    CertTableElementIndex = 7;
    ElePtr = &CertTable[CertVer-1][CertTableElementIndex][0];
    Res = IpBase_BerGetElement(&Workspace, &EleDescKeyMat, ElePtr, CertTableElementDepth[CertTableElementIndex], DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* pub key mat */
      if(TLS_KEY_EXCH_TYPE_ECDH == PubKeyType)
      {
        /* pub key mat ecc */
        if(0 == DataPtr[EleDescKeyMat.ContentIdx])
        {
          CertDesc->KeyMain.ContOffs = (uint16)(EleDescKeyMat.ContentIdx +1);  /* inner offset */
          CertDesc->KeyMain.ContLen  = (uint16)((EleDescKeyMat.EndIdx - EleDescKeyMat.ContentIdx) -1);
        }
        else
        {
          CertDesc->KeyMain.ContOffs = (uint16)(EleDescKeyMat.ContentIdx);  /* inner offset */
          CertDesc->KeyMain.ContLen  = (uint16)(EleDescKeyMat.EndIdx - EleDescKeyMat.ContentIdx);
        }
        CertDesc->CertFoundElements |= TLS_CERT_ELE_KEY_ECC_PUB_KEY;
      }
      else  /* (TLS_KEY_EXCH_TYPE_RSA == PubKeyType) */
      {
        uint32        RsaLocDataStart = EleDescKeyMat.ContentIdx +1;
        uint32        RsaLocDataLen   = EleDescKeyMat.EndIdx - (RsaLocDataStart);
        const uint8  *RsaLocDataPtr   = (const uint8*)&DataPtr[RsaLocDataStart];  /* inner offset */

        /* parse for the RSA key modulus */
        Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrCertRsaPubKey[0], 2, RsaLocDataPtr, RsaLocDataLen);
        if( IPBASE_E_OK == Res )
        {
          /* cert signature */
          CertDesc->KeyMain.ContOffs = (uint16)(EleDesc.ContentIdx + RsaLocDataStart +1);  /* inner offset */
          CertDesc->KeyMain.ContLen  = (uint16)(EleDesc.EndIdx - (EleDesc.ContentIdx +1));
          CertDesc->CertFoundElements |= TLS_CERT_ELE_KEY_RSA_MODULUS;
        }
        else
        {
          return E_NOT_OK;
        }

        /* parse for the RSA key exponent */
        Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrCertRsaPubExp[0], 2, RsaLocDataPtr, RsaLocDataLen);
        if( IPBASE_E_OK == Res )
        {
          /* cert signature */
          CertDesc->KeyExp.ContOffs = (uint16)(EleDesc.ContentIdx + RsaLocDataStart);
          CertDesc->KeyExp.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
          CertDesc->CertFoundElements |= TLS_CERT_ELE_KEY_RSA_PUB_EXP;
        }
        else
        {
          return E_NOT_OK;
        }
      }
    }
    else
    {
      return E_NOT_OK;
    }

    /* # Find the position of the certificate signature algorithm */
    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrCertSignAlg[0], 2, DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* cert signature algorithm */
      CertDesc->CertSignAlgId.ContOffs = (uint16)EleDesc.ContentIdx;  /* inner offset */
      CertDesc->CertSignAlgId.ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
      CertDesc->CertFoundElements |= TLS_CERT_ELE_CERT_SIGN_ALG_ID;
    }
    else
    {
      return E_NOT_OK;
    }

    Res = IpBase_BerGetElement(&Workspace, &EleDesc, &EleNrCertSign[0], 2, DataPtr, LocDataLen);
    if( IPBASE_E_OK == Res )
    {
      /* cert signature */
      CertDesc->CertSignature.ContOffs = (uint16)(EleDesc.ContentIdx +1);  /* inner offset */
      CertDesc->CertSignature.ContLen  = (uint16)(EleDesc.EndIdx - (EleDesc.ContentIdx +1));
      CertDesc->CertFoundElements |= TLS_CERT_ELE_CERT_SIGNATURE;
    }
    else
    {
      return E_NOT_OK;
    }
  }

  while(HashInfo != TLS_HASH_INFO_NONE)
  {
    uint8 *TgtPtr;
    if(((HashInfo & TLS_HASH_INFO_ISSUER) == TLS_HASH_INFO_ISSUER ) && ((CertDesc->CertFoundElements & TLS_CERT_ELE_ISSUER) == TLS_CERT_ELE_ISSUER))
    {
      /* TLS_HASH_INFO_ISSUER */
      ObjLen    = CertDesc->Issuer.ContLen;
      ObjOffs   = CertDesc->Issuer.ContOffs;
      TgtPtr    = CertDesc->Hash8Sha1Issuer;
      HashInfo -= TLS_HASH_INFO_ISSUER;
    }
    else if(((HashInfo & TLS_HASH_INFO_SUBJECT) == TLS_HASH_INFO_SUBJECT) && ((CertDesc->CertFoundElements & TLS_CERT_ELE_SUBJECT) == TLS_CERT_ELE_SUBJECT))
    {
      /* TLS_HASH_INFO_SUBJECT */
      ObjLen    = CertDesc->Subject.ContLen;
      ObjOffs   = CertDesc->Subject.ContOffs;
      TgtPtr    = CertDesc->Hash8Sha1Subject;
      HashInfo -= TLS_HASH_INFO_SUBJECT;
    }
    else
    {
      return E_NOT_OK;
    }

    /* hash issuer or subject chapter */
    {
      uint8 ElementHash[TLS_HASH_SHA1_LEN];
      eslt_WorkSpaceSHA1  WorkSpaceSha1;

      /* hash the subject */
      (void)esl_initWorkSpaceHeader( &WorkSpaceSha1.header, ESL_MAXSIZEOF_WS_SHA1, NULL_PTR );
      (void)esl_initSHA1( &WorkSpaceSha1 );
      (void)esl_updateSHA1( &WorkSpaceSha1, (uint16)ObjLen, &DataPtr[ObjOffs] );
      (void)esl_finalizeSHA1( &WorkSpaceSha1, &ElementHash[0] );

      /* copy first 8 bytes of the hash */
      IpBase_Copy( (IpBase_CopyDataType*)(TgtPtr),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
        (const IpBase_CopyDataType*)(&ElementHash[0]),  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
        TLS_CERT_INT_HASH_LEN );
    }

    /* Hash the whole certificate for use with the 'trusted CA' extension */
    {
      eslt_WorkSpaceSHA1  workSpaceSha1;
      uint16              certLen  = (uint16)LocDataLen;
      const uint8        *certPtr  = DataPtr;

      /* hash the subject */
      (void)esl_initWorkSpaceHeader( &workSpaceSha1.header, ESL_MAXSIZEOF_WS_SHA1, NULL_PTR );
      (void)esl_initSHA1( &workSpaceSha1 );
      (void)esl_updateSHA1( &workSpaceSha1, certLen, certPtr );
      (void)esl_finalizeSHA1( &workSpaceSha1, &CertDesc->HashSha1Cert[0] );
    }
  }

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (TLS_SUPPORT_CRL == STD_ON)
/***********************************************************************************************************************
 *  Tls_VParseBerStruct
 **********************************************************************************************************************/
/*! \brief         Parse a BER encoded struct for internal elements.
 *  \param[in]     DataPtr               Pointer to the BER encoded block
 *  \param[in]     DataLen               Length of the BER encoded block
 *  \param[out]    BerStructDescPtr      Pointer to array of struct containing element positions and lengths
 *  \param[in]     BerChapterTablePtr    Pointer to array with chapter numbers
 *  \param[in]     BerChapterTableSize   Number of chapters in the array BerChapterTablePtr
 *  \param[in]     BerChapterTableDepth  Maximum chapter depth
 *  \param[in]     BerChapterDepthPtr    Pointer to array with depth of each chapter
 *  \param[out]    FoundElementFlagsPtr  Pointer to flag-field containing the flags of the found chapters
 *  \return        void
 *  \context       task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VParseBerStruct(
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen,
    P2VAR(Tls_Cert_EleInfo, AUTOMATIC, TLS_APPL_DATA) BerStructDescPtr,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) BerChapterTablePtr,
    uint8 BerChapterTableSize,
    uint8 BerChapterTableDepth,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) BerChapterDepthPtr,
    P2VAR(uint32, AUTOMATIC, TLS_APPL_DATA) FoundElementFlagsPtr
    )
{
  uint32 FoundElements = 0;
  uint32 CurrentElementFlag = 0x1;

  if(0 == DataPtr[0])
  {
    /* invalid input */
    *FoundElementFlagsPtr = 0;
    return;
  }

  /* now parse the certificate */
  {
    /*
    CrlCh1;            -> chapter 1
    tbsCertList;       -> chapter 1.1
    MySignAlgId;       -> chapter 1.1.2
    Issuer;            -> chapter 1.1.3
    ThisUpdate;        -> chapter 1.1.4
    NextUpdate;        -> chapter 1.1.5
    Subject;           -> chapter 1.1.6
    RevokedCerts;      -> chapter 1.1.7
    CrlSignAlgId;      -> chapter 1.2
    CrlSignature;      -> chapter 1.3
    */

    CONST(uint8, TLS_CONST)     MaxEleDepth = 5;
    IpBase_ReturnType           Res;
    IpBase_BerWorkspaceType     Workspace;
    IpBase_BerStackElementType  BerStack[5];  /* max depth of one element / chapter */
    IpBase_BerElementType       EleDesc;
    uint8_least                 EleIndex;

    IpBase_BerInitWorkspace(&Workspace, &BerStack[0], MaxEleDepth); /* only needed once */

    for(EleIndex = 0; EleIndex < BerChapterTableSize; EleIndex++)
    {
      Tls_Cert_EleInfo * CurrentEleDescPtr = &BerStructDescPtr[EleIndex];

      Res = IpBase_BerGetElement(&Workspace, &EleDesc, &BerChapterTablePtr[EleIndex * BerChapterTableDepth], BerChapterDepthPtr[EleIndex], DataPtr, DataLen );
      if( IPBASE_E_OK == Res )
      {
        /* now, EleDesc contains the index information */
        CurrentEleDescPtr->ContOffs = (uint16)EleDesc.ContentIdx;
        CurrentEleDescPtr->ContLen  = (uint16)(EleDesc.EndIdx - EleDesc.ContentIdx);
        FoundElements |= CurrentElementFlag;
      }
      else
      {
        /* error, an element could not be found in the given byte array */
        return;
      }

      CurrentElementFlag = CurrentElementFlag << 1;
    }

    *FoundElementFlagsPtr = FoundElements;
  }
}

/***********************************************************************************************************************
 *  Tls_VHashSomeDataWithSha1
 **********************************************************************************************************************/
/*! \brief         Hash a given data block using SHA1.
 *  \param[in]     DataPtr               Pointer to the data block
 *  \param[in]     DataLen               Length of the data block
 *  \param[out]    HashOutputPtr         Pointer to the hash output
 *  \return        void
 *  \context       task level
 **********************************************************************************************************************/
STATIC void Tls_VHashSomeDataWithSha1(
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) HashOutputPtr )
{
  eslt_WorkSpaceSHA1  WorkSpaceSha1;

  /* init ws header */
  (void)esl_initWorkSpaceHeader(&WorkSpaceSha1.header, ESL_MAXSIZEOF_WS_SHA1, NULL_PTR);
  /* init sha1 */
  (void)esl_initSHA1( &WorkSpaceSha1 );
  /* update sha1 */
  (void)esl_updateSHA1( &WorkSpaceSha1, DataLen, DataPtr );
  /* finalize sha1 */
  (void)esl_finalizeSHA1( &WorkSpaceSha1, HashOutputPtr );
}

/***********************************************************************************************************************
 *  Tls_VParseCrl
 **********************************************************************************************************************/
/*! \brief         Parse CRL that was read from NVM and store information in global variable
 *  \context       task level
 *  \param[in]     CrlId        Index of the current CRL
 *  \param[in]     DataPtr      Pointer to the CRL
 *  \param[in]     DataLen      Length of the CRL (NVM block) byte array
 *  \return        E_OK         CRL could be parsed
 *  \return        E_NOT_OK     CRL could not be parsed successfully
 *  \context       task level
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VParseCrl(
    uint8 CrlId,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen )
{
  Std_ReturnType RetVal;

  CONST(uint8, TLS_CONST) Tls_CrlChapters[TLS_CRL_NUM_OF_CRL_CHAPTERS][TLS_CRL_MAX_CHAPTER_DEPTH] =
  {
    { 1, 0, 0 },  /* chapter 1    : CrlCh1 */
    { 1, 1, 0 },  /* chapter 1.1  : tbsCertList */
    { 1, 1, 2 },  /* chapter 1.1.2: MySignAlgId */
    { 1, 1, 3 },  /* chapter 1.1.3: Issuer */
    { 1, 1, 4 },  /* chapter 1.1.4: ThisUpdate */
    { 1, 1, 5 },  /* chapter 1.1.5: NextUpdate */
    { 1, 1, 6 },  /* chapter 1.1.6: RevokedCerts */
    { 1, 1, 7 },  /* chapter 1.1.7: Extensions */
    { 1, 2, 0 },  /* chapter 1.2  : CrlSignAlgId */
    { 1, 3, 0 }   /* chapter 1.3  : CrlSignature */
  };
  CONST(uint8, TLS_CONST) Tls_CrlChapterDepth[TLS_CRL_NUM_OF_CRL_CHAPTERS] =
  { 1, 2, 3, 3, 3, 3, 3, 3, 2, 2 };

  Tls_VParseBerStruct( DataPtr, DataLen,
    Tls_CfgCrlElePosArrays[CrlId], &Tls_CrlChapters[0][0], TLS_CRL_NUM_OF_CRL_CHAPTERS, TLS_CRL_MAX_CHAPTER_DEPTH,
    &Tls_CrlChapterDepth[0], &Tls_CrlDescs[CrlId].CrlFoundElements );

  if((Tls_CrlDescs[CrlId].CrlFoundElements & (0x00000001 << TLS_CRL_ELE_ISSUER)) != 0)
  {
    uint8 HashOutput[TLS_HASH_SHA1_LEN];
    Tls_VHashSomeDataWithSha1(
      &DataPtr[(Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_ISSUER]).ContOffs],
    (Tls_CfgCrlElePosArrays[CrlId][TLS_CRL_ELE_ISSUER]).ContLen,
    &HashOutput[0] );

  /* copy first 8 bytes */
    IpBase_Copy( (IpBase_CopyDataType*)&(Tls_CrlDescs[CrlId].Hash8Sha1Issuer[0]),
      (const IpBase_CopyDataType*)&HashOutput[0], TLS_CERT_INT_HASH_LEN );

    RetVal = E_OK;
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  return RetVal;
}
#endif  /* (TLS_SUPPORT_CRL == STD_ON) */

/***********************************************************************************************************************
 *  Tls_VHs_HashRxMsg
 **********************************************************************************************************************/
/*! \brief      Read the key algorithm from an OID (object ID)
 *  \param[in]  DataPtr      Pointer to the key type element
 *  \param[in]  DataLen      Length of the key type element
 *  \return     key identifier, set to TLS_KEY_ALG_TYPE_INVALID if key type could not be identified
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(uint8, TLS_CODE) Tls_VReadKeyAlgId(
    CONSTP2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen )
{
  /* cert key type */
  CONST(uint8, TLS_CONST) IdEcPublicKey[7]   = { 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01 };
  CONST(uint8, TLS_CONST) IdRsaEncryption[9] = { 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01 };

  /* compare */
  if((DataLen >= 7) && (IPBASE_CMP_EQUAL == IpBase_StrCmpLen(DataPtr, &IdEcPublicKey[0], 7)))  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
  {
    return TLS_KEY_ALG_TYPE_ECC;
  }
  else if((DataLen >= 9) && (IPBASE_CMP_EQUAL == IpBase_StrCmpLen(DataPtr, &IdRsaEncryption[0], 9)))  /* PRQA S 3415 */ /* MD_TLS_12.4_3415 */
  {
    return TLS_KEY_ALG_TYPE_RSA;
  }
  else
  {
    /* invalid or not supported key type */
    return TLS_KEY_ALG_TYPE_INVALID;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VHs_HashRxMsg
 **********************************************************************************************************************/
/*! \brief      hash rx handshake message
 *  \param[in]  TConnId      Connection identifier
 *  \param[in]  DataPtr      Pointer to the received message
 *  \param[in]  DataLen      Length of the received message
 *  \return     void
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_HashRxMsg(
    uint8 TConnId,
    CONSTP2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen )
{
  /* hash received HS messages (parts), but only before client_ccs was sent */
  if( (0 != DataLen) && (TLS_HS_STATE_CCS_SENT >= Tls_HandShakeState[TConnId]) )
  {

    Tls_VHs_HashMsg( TConnId, DataPtr, DataLen );
    /* If there occurs an error during hashing, the TLS PROTOCOL will recognize it later. Therefore there is no need to
       handled the error here. */
  }
}

/***********************************************************************************************************************
 *  Tls_VHs_HashMsg
 **********************************************************************************************************************/
/*! \brief      hash handshake message
 *  \param[in]  TConnId      Connection identifier
 *  \param[in]  DataPtr      Pointer to the message
 *  \param[in]  DataLen      Length of the message
 *  \return     void
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VHs_HashMsg(
    uint8 TConnId,
    CONSTP2CONST(uint8, AUTOMATIC, TLS_CONST) DataPtr,
    uint16 DataLen )
{
  TLS_LOG_HS_DIV_1("Tls_VHs_HashMsg: hash msg with len %d", DataLen);

  switch(Tls_HsHashAlg[TConnId])
  {
#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
  case TLS_HS_HASH_ALG_MD5SHA1:
    {
      /* MD5 + SHA1 hash update */
      (void)esl_updateMD5( &Tls_WorkSpaceMd5_Hs1[TConnId], DataLen, DataPtr );
      (void)esl_updateSHA1( (eslt_WorkSpaceSHA1*)&Tls_WorkSpaceShaX_Hs1[TConnId], DataLen, DataPtr );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      break;
    }
#endif
#if (TLS_SUPPORT_PROT_VER_TLS12 == STD_ON)
  case TLS_HS_HASH_ALG_SHA256:
    {
      /* SHA256 hash update */
      (void)esl_updateSHA256( (eslt_WorkSpaceSHA256*)&Tls_WorkSpaceShaX_Hs1[TConnId], DataLen, DataPtr );  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_ws */
      break;
    }
#endif
  default:
    {
      /* error, inv alg */
      return;
    }
  }

  /* If there occurs an error during hashing, the TLS PROTOCOL will recognize it later. Therefore there is no need to
  handled the error here. */
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Tls_VConvertTimeDT2S
 **********************************************************************************************************************/
/*! \brief      convert time from Date-Time format (sec, min, hour, day, month, year) to Unix format (sec since
 *              01-01-1970)
 *  \param[in]  TimeDTPtr    pointer to time in Date-Time format
 *  \param[out] TimePtr      pointer to where the time shall be stored (in seconds since 01-01-1970)
 *  \return     none
 *  \context    -
 **********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VConvertTimeDT2S(
    P2CONST(Tls_TimeDTType, AUTOMATIC, TLS_APPL_DATA) TimeDTPtr,
    P2VAR(uint32, AUTOMATIC, TLS_APPL_DATA) TimeSPtr )
{
  /* - Every 4th year is a leap year, every hundret years the leap year is skipped, every 400 years the leap year takes
       place.
     - Tls_TimeDTType.Year starts in 1900 */

  uint32                  TotalSecs;
  uint8                   NumOfLeapYears;  /* complete leap years since 1970 (not including current year) */
  uint16                  DaysInYear;      /* completed days in the current year */
  CONST(uint8, TLS_CONST) DaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  {
    uint8 i;

    DaysInYear = 0;
    for(i=0; i < TimeDTPtr->Mon; i++)
    {
      DaysInYear += DaysInMonth[i];
    }
    DaysInYear += (TimeDTPtr->Mday -1);

    /* inc the day counter for each leap year since 1970 */
    if(0 == (((TimeDTPtr->Year -70) +2) % 4))
    {
      /* this is a leap year */
      if(TimeDTPtr->Mon > 1) /* march to december */
      {
        DaysInYear += 1;  /* add a leap day */
      }
    }
  }
  {
    NumOfLeapYears = (uint8)(((TimeDTPtr->Year -70) +1) / 4);
  }

  TotalSecs = 0;
  TotalSecs += (uint32)((((((uint32)TimeDTPtr->Year) -70) *365) + NumOfLeapYears) * 86400);  /* add years */
  TotalSecs += (uint32)(DaysInYear * 86400);       /* add days in this year */
  TotalSecs += (uint32)(TimeDTPtr->Hour * 3600);   /* add hours */
  TotalSecs += (uint32)(TimeDTPtr->Min * 60);      /* add minutes */
  TotalSecs += (uint32)(TimeDTPtr->Sec);           /* add seconds */

  *TimeSPtr = TotalSecs;
}

#if (TLS_SUPPORT_OCSP == STD_ON)
/***********************************************************************************************************************
 *  Tls_VCopyPubKeyFromCert
 **********************************************************************************************************************/
/*! \brief      copy cert public key from received cert chain for later usage
 *  \param[in]  TConnId       Tls connection id
 *  \param[in]  CertPtr       pointer to received cert
 *  \param[in]  CertDescrPtr  pointer to descriptor of the received cert
 *  \return     E_OK          key could be copied <br>
 *              E_NOT_OK      key could not be copied
 *  \context    -
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VCopyPubKeyFromCert(
    uint8 TConnId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) CertPtr,
    P2CONST(Tls_Cert_Descr, AUTOMATIC, TLS_APPL_DATA) CertDescrPtr )
{
  switch(Tls_Hs_KeyExchType[TConnId])
  {
#if (TLS_KEY_EXCH_RSA_SUPPORT == STD_ON)
  case(TLS_KEY_EXCH_TYPE_RSA):
    {
      /* Indexed position of the current public key in the TLS internal key storage element */
      uint8  GlobalCertIndex = (TConnId * (TLS_CFG_OCSP_MAX_CERT_CHAIN_LEN - 1)) + (Tls_Ocsp_CertChainLen[TConnId] -2);
      /* get key size */
      uint16 KeySize         = CertDescrPtr->KeyMain.ContLen;
      uint16 KeyPos          = CertDescrPtr->KeyMain.ContOffs;
      /* set pointers */
      uint8 *StoragePtr      = &Tls_Ocsp_CertChainKeys[GlobalCertIndex * TLS_CFG_MAX_SERVER_CERT_KEY_SIZE_BYTE];
      uint8 *SrcPtr          = &CertPtr[KeyPos];

      TLS_LOG_HS_DIV_1("Tls_VHs_RxServerCert: RSA: copy public key from cert, GlobalCertIndex is now %d", GlobalCertIndex);
      IpBase_Copy((IpBase_CopyDataType*)StoragePtr, (IpBase_CopyDataType*)SrcPtr, KeySize);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */

      {
        /* check RSA exponent to be 0x010001 */
        uint8* ExpPtr;
        uint8  ExpRefArray[3] = {1,0,1};
        if(CertDescrPtr->KeyExp.ContLen != 3)
        {
          /* error, currently not supportet. Only support for exponents with length = 3 is currently implemented. */
          return E_NOT_OK;
        }
        ExpPtr = &CertPtr[CertDescrPtr->KeyExp.ContOffs];
        if(IPBASE_CMP_EQUAL != IpBase_StrCmpLen(ExpPtr, &ExpRefArray[0], 3))
        {
          /* error, currently not supportet. Only support for exponent 0x010001 is currently implemented. */
          return E_NOT_OK;
        }
      }
      break;
    }
#endif  /* TLS_KEY_EXCH_RSA_SUPPORT */
#if (TLS_KEY_EXCH_ECDH_SUPPORT == STD_ON)
  case(TLS_KEY_EXCH_TYPE_ECDH):
  case(TLS_KEY_EXCH_TYPE_ECDHE):
    {
      /* Indexed position of the current public key in the TLS internal key storage element */
      uint8  GlobalCertIndex = (TConnId * (TLS_CFG_OCSP_MAX_CERT_CHAIN_LEN - 1)) + (Tls_Ocsp_CertChainLen[TConnId] -2);
      /* get key size */
      uint16 KeySize         = CertDescrPtr->KeyMain.ContLen;
      uint16 KeyPos          = CertDescrPtr->KeyMain.ContOffs;
      /* set pointers */
      uint8 *StoragePtr      = &Tls_Ocsp_CertChainKeys[GlobalCertIndex * TLS_CFG_MAX_SERVER_CERT_KEY_SIZE_BYTE];
      uint8 *SrcPtr          = &CertPtr[KeyPos];

      TLS_LOG_HS_DIV_1("Tls_VHs_RxServerCert: ECC: copy public key from cert, GlobalCertIndex is now %d", GlobalCertIndex);
      IpBase_Copy((IpBase_CopyDataType*)StoragePtr, (IpBase_CopyDataType*)SrcPtr, KeySize);  /* PRQA S 0310 */ /* MD_TLS_11.4_0310_copy */  /* PRQA S 3305 */ /* MD_TLS_3305_copy */
      break;
    }
#endif  /* TLS_KEY_EXCH_ECDH_SUPPORT */
  default:
    {
      /* error */
      break;
    }
  }

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/**********************************************************************************************************************
 *  Tls_VWriteBufSegment
 *********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VInitBufSegment(
                        P2VAR(Tls_BufSegMgtType, AUTOMATIC, TLS_APPL_DATA) BufSegPtr)
{
  BufSegPtr->Idx = 0;
  BufSegPtr->Lvl = 0;
}

/**********************************************************************************************************************
 *  Tls_VWriteBufSegment
 *********************************************************************************************************************/
/* PRQA S 6080 MSR_STMIF */ /* MD_MSR_STMIF */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_VWriteBufSegment(
                        Tls_TConnIdType TConnId,
                        P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) SrcBufPtr,
                        uint16 SrcBufLen,
                        P2VAR(Tls_BufSegMgtType, AUTOMATIC, TLS_APPL_DATA) TgtBufSegPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

  if ( 0u != SrcBufLen )
  { /* data received */
    /* check if buffer struct is alredy used */
    if ( 0u == TgtBufSegPtr->Lvl )
    { /* buffer struct unused, fill first element */
      TgtBufSegPtr->BufSegs[TgtBufSegPtr->Idx].Len = SrcBufLen;
      TgtBufSegPtr->BufSegs[TgtBufSegPtr->Idx].DataPtr = SrcBufPtr;
      TgtBufSegPtr->Lvl = 1;
      retVal = E_OK;
    }
    else
    { /* buffer struct contains elements (at least one segment received before) */
      uint16 lastIdx;
      uint16 writeIdx;
      uint16 curLvl = (uint16)(TgtBufSegPtr->Lvl - 1u);  /* level index */

      /* get current level-index of the last element */
      if ( (TgtBufSegPtr->Idx + curLvl) >= TLS_VBUF_SEG_NUM_TOTAL )
      { /* wrap around necessary */
        lastIdx = curLvl - (TLS_VBUF_SEG_NUM_TOTAL - TgtBufSegPtr->Idx);
      }
      else
      { /* no wrap around necessary */
        lastIdx = TgtBufSegPtr->Idx + curLvl;
      }

      /* check if new data are available */
      if ( &TgtBufSegPtr->BufSegs[lastIdx].DataPtr[0] != &SrcBufPtr[0] )  /* PRQA S 3689 */ /* MD_TLS_21.1_3689 */
      { /* new data shall be added */
        /* check if new segment is in same memory segment like last segment */
        if ( &TgtBufSegPtr->BufSegs[lastIdx].DataPtr[TgtBufSegPtr->BufSegs[lastIdx].Len] == &SrcBufPtr[0] )  /* PRQA S 3689 */ /* MD_TLS_21.1_3689 */
        { /* new segment is in same memory segment */
          /* increment length only */
          TgtBufSegPtr->BufSegs[lastIdx].Len += SrcBufLen;  /* PRQA S 3689 */ /* MD_TLS_21.1_3689 */
          retVal = E_OK;
        }
        else
        { /* new segment available */
          if ( TLS_VBUF_SEG_NUM_TOTAL > TgtBufSegPtr->Lvl )
          { /* segmented buffer struct size sufficient */
            /* find next free segment */
            if ( (TgtBufSegPtr->Idx + TgtBufSegPtr->Lvl) >= TLS_VBUF_SEG_NUM_TOTAL )
            { /* wrap around necessary */
              writeIdx = TgtBufSegPtr->Lvl - (TLS_VBUF_SEG_NUM_TOTAL - TgtBufSegPtr->Idx);
            }
            else
            { /* no wrap around necessary */
              writeIdx = TgtBufSegPtr->Idx + TgtBufSegPtr->Lvl;
            }

            TgtBufSegPtr->BufSegs[writeIdx].Len = SrcBufLen;      /* PRQA S 3689 */ /* MD_TLS_21.1_3689 */
            TgtBufSegPtr->BufSegs[writeIdx].DataPtr = SrcBufPtr;  /* PRQA S 3689 */ /* MD_TLS_21.1_3689 */
            TgtBufSegPtr->Lvl++;
            retVal = E_OK;
          }
        }
      }
      else
      {
        retVal = E_OK;
      }
    }
  }
  else
  { /* no new data received */
    retVal = E_OK;
  }

  if(E_OK == retVal)
  {
    Tls_VGetPbufFromBufSegment(TgtBufSegPtr, &Tls_RxPbufList[TConnId *2]);
  }

  return retVal;
}
/* PRQA L:MSR_STMIF */

/**********************************************************************************************************************
 *  Tls_VGetPbufFromBufSegment
 *********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VGetPbufFromBufSegment(
                        P2CONST(Tls_BufSegMgtType, AUTOMATIC, TLS_APPL_DATA) BufSegPtr,
                        P2VAR(IpBase_PbufType, AUTOMATIC, TLS_APPL_VAR) PbufPtr )
{
  /* ASSUMPTION: the Vector TCP is used and supports only a maximum of two buffer fragments */

  uint16 FirstLevelIndex  = BufSegPtr->Idx;
  uint16 SecondLevelIndex = (uint16)((BufSegPtr->Idx +1) % TLS_VBUF_SEG_NUM_TOTAL);

  if(BufSegPtr->Lvl == 0)
  {
    PbufPtr->len = 0;
    PbufPtr->totLen = 0;
    return;
  }

  PbufPtr[0].payload = BufSegPtr->BufSegs[FirstLevelIndex].DataPtr;
  PbufPtr[0].len     = BufSegPtr->BufSegs[FirstLevelIndex].Len;
  if(BufSegPtr->Lvl == 1)
  {
    PbufPtr[0].totLen = BufSegPtr->BufSegs[FirstLevelIndex].Len;
  }
  else
  {
    uint32 TotLen = BufSegPtr->BufSegs[FirstLevelIndex].Len + BufSegPtr->BufSegs[SecondLevelIndex].Len;  /* PRQA S 0781 */ /* MD_TLS_5.6_0781 */
    PbufPtr[1].payload = BufSegPtr->BufSegs[SecondLevelIndex].DataPtr;
    PbufPtr[1].len     = BufSegPtr->BufSegs[SecondLevelIndex].Len;
    PbufPtr[1].totLen  = TotLen;
    PbufPtr[0].totLen  = TotLen;
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Tls_VReleaseBufSegment
 *********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VReleaseBufSegment(
                        Tls_TConnIdType TConnId,
                        P2VAR(Tls_BufSegMgtType, AUTOMATIC, AUTOMATIC) BufSegPtr,
                        uint32 Len)
{
  uint32 LenToRelease;

  TLS_VENTER_CRITICAL_SECTION();  /* lock interrupts */  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  LenToRelease = Len;

  while ( (0u < Len) && (0u < BufSegPtr->Lvl) )
  {
    if ( Len < BufSegPtr->BufSegs[BufSegPtr->Idx].Len )
    { /* release buffer in current element only */
      BufSegPtr->BufSegs[BufSegPtr->Idx].Len -= (uint16)Len;
      BufSegPtr->BufSegs[BufSegPtr->Idx].DataPtr = &BufSegPtr->BufSegs[BufSegPtr->Idx].DataPtr[Len];
      Len = 0u;
    }
    else
    { /* release complete element */
      Len -= BufSegPtr->BufSegs[BufSegPtr->Idx].Len;
      BufSegPtr->BufSegs[BufSegPtr->Idx].Len = 0u;

      if ( (BufSegPtr->Idx + 1u) == TLS_VBUF_SEG_NUM_TOTAL )
      { /* wrap around necessary */
        BufSegPtr->Idx = 0u;
      }
      else
      { /* no wrap around necessary */
        BufSegPtr->Idx++;
      }
      BufSegPtr->Lvl--;
    }
  }

  /* update internal Pbuf struct */
  Tls_VGetPbufFromBufSegment(BufSegPtr, &Tls_RxPbufList[TConnId *2]);

  TLS_VLEAVE_CRITICAL_SECTION();  /* unlock interrupts */  /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* release buffer in TCP */
  TcpIp_Tcp_TlsReceived(Tls_CfgTlsConn2SockHndMap[TConnId], LenToRelease);
}

/**********************************************************************************************************************
 *  Tls_VResetSockConnMappings
 *********************************************************************************************************************/
STATIC FUNC(void, TLS_CODE) Tls_VResetSockConnMappings(
    TcpIp_SocketIdType SocketId )
{
  if(TLS_COND_VALID_SOCKID(SocketId))
  {
    Tls_TConnIdType ElementIdx;
    Tls_TConnIdType TConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* reset the socket and connection mappings */
    Tls_SockHnd2TlsConnMap[SocketId]   = TLS_INV_CONN_ID;
    Tls_CfgTlsConn2SockHndMap[TConnId] = TCPIP_INV_SOCK_IDX;

    /* find the matching Tls_Sock2ConnMapper element and delete element values */
    for(ElementIdx = 0; ElementIdx < TLS_CFG_NUM_CONNECTIONS; ElementIdx++)
    {
      if(SocketId == Tls_Sock2ConnMapper[ElementIdx].SocketId)
      {
        /* A free mapper element found */
        Tls_Sock2ConnMapper[ElementIdx].SocketId        = TCPIP_INV_SOCK_IDX;
        Tls_Sock2ConnMapper[ElementIdx].TlsConnectionId = TLS_INV_CONN_ID;
        Tls_Sock2ConnMapper[ElementIdx].RxBufferSize    = 0;
        Tls_Sock2ConnMapper[ElementIdx].TxBufferSize    = 0;
        break;
      }
    }
  }
  else
  {
    /* socket id is invalid -> do nothing */
  }
}

#if ((defined TLS_PROCESSOR_CANOEEMU) && (TLS_VENABLE_CANOE_WRITE_STRING == STD_ON))
/***********************************************************************************************************************
 *  Tls_VConvertByteArray2HexString
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_VConvertByteArray2HexString(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) ArrayPtr,
    uint8 ArrayLen )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least                             arrayIndex;
  P2VAR(sint8, AUTOMATIC, TLS_APPL_DATA)  writePtr      = &Tls_CanoeWriteStr[0];
  P2VAR(sint8, AUTOMATIC, TLS_APPL_DATA)  bufferEndPPtr = writePtr + sizeof(Tls_CanoeWriteStr);

  /* ----- Checks --------------------------------------------------------- */
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  TLS_ASSERT(TLS_COND_INIT)
  /* Check parameter 'ArrayPtr' */
  TLS_ASSERT(TLS_COND_VALID_PTR(ArrayPtr))
  /* Check parameter 'MsLenPtr' */
  TLS_ASSERT(ArrayLen < (TLS_UINT8_MAX_VALUE - 2))  /* two bytes for string termination ('\n' and null terminator) */

  /* ----- Implementation ------------------------------------------------- */

  /* #20 Write the hex value for each byte and terminate the string. */
  for (arrayIndex = 0; arrayIndex < ArrayLen; arrayIndex++)
  {
    if ((writePtr + 2) < bufferEndPPtr)  /* two bytes for string termination ('\n' and null terminator) */
    {
      writePtr += _snprintf_s(writePtr, 256, 256, "%02X", ArrayPtr[arrayIndex]);
    }
  }
  writePtr += _snprintf_s(writePtr, 256, 256, "\n");
}
#endif

#if ((defined TLS_SUPPORT_MASTER_SECRET_ACCESS) && (TLS_SUPPORT_MASTER_SECRET_ACCESS == STD_ON))
/***********************************************************************************************************************
 *  Tls_GetMasterSecret
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TLS_CODE) Tls_GetMasterSecret(
    Tls_SocketIdType SocketId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) *MsDataPtr,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) MsLenPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal   = E_NOT_OK;
  uint8          errorId  = TLS_E_NO_ERROR;
  uint8          tConnId;

  /* ----- Development Error Checks ------------------------------------- */
#if (TLS_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  /* Check initialization state of the component */
  if (TLS_COND_UNINIT)
  {
    errorId = TLS_E_NOT_INITIALIZED;
  }
  /* Check parameter 'SocketId' */
  else if (TLS_COND_INVALID_SOCKID(SocketId))
  {
    errorId = TLS_E_INV_SOCK_ID;
  }
  /* Check parameter 'MsDataPtr' */
  else if (TLS_COND_INVALID_PTR(MsDataPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  /* Check parameter 'MsLenPtr' */
  else if (TLS_COND_INVALID_PTR(MsLenPtr))
  {
    errorId = TLS_E_INV_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    tConnId = Tls_VCfgGetSockId2TlsConn(SocketId);

    /* #20 Read the master secret. */
    *MsDataPtr = &Tls_SecParamTxActivePtr[tConnId]->master_secret[0];
    *MsLenPtr = TLS_MASTER_SECRET_LEN;
    retVal    = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TLS_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report an error if necessary. */
  if (errorId != TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID, TLS_API_ID_GET_MASTER_SECRET, errorId);
  }
#endif

  return retVal;
}
#endif

#define TLS_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
   MD_TLS_5.6_0781:
     Reason:      Readablitiy is increased by using well known names.
     Risk:        Elements can be confused with each other.
     Prevention:  Covered by code review.
   MD_TCPIP_6.3_5013:
     Reason:      Usage of basic types is defined in SWS_SoAd
     Risk:        problem with different platforms
     Prevention:  not possible
   MD_TLS_8.7_3218:
     Reason:      Data is not defined within the funtion scope to increase the readibility of the function
                  implementation.
     Risk:        No functional risk.
     Prevention:  Not required.
   MD_TCPIP_8.12_3684:
      Reason:     The arrays in file TcpIp_PBcfg.h and TcpIp_Lcfg.h are declared as extern without size specification
                  since their size depend on the configuration (e.g. in GENy).
      Risk:       There is no risk because these arrays are only used by TcpIp.
      Prevention: Covered by code review.
   MD_TLS_11.4_0310_sock:
      Reason:     Tls_SockAddrType includes address family to differ between IPv4 and IPv6. The corresponding
                  structs also start with the family field so casting is possible.
      Risk:       Only casting from IpBase_SockAddrInType or Tls_SockAddrIn6Type to Tls_SockAddrType is allowed.
      Prevention: Covered by code review.
   MD_TLS_11.4_0310_copy:
      Reason:     The copy function uses a different base type as the local element.
      Risk:       The copy function might cause access problems on some platforms if elements are not aligned properly.
      Prevention: Covered by code review.
   MD_TCPIP_11.4_0310_compare:
      Reason:     The compare function uses a different base type as the local element.
      Risk:       The compare function might cause access problems on some platforms if elements are not aligned properly.
      Prevention: Covered by code review.
   MD_TLS_11.4_0310_struct:
      Reason:     Struct-mapping on a byte array element (e.g. a stream)
      Risk:       This might cause access problems on some platforms if elements are not aligned properly.
      Prevention: Covered by code review.
   MD_TLS_11.4_0310_keymat:
      Reason:     The key material is generated into a big byte-array, and then parts of it are used for different purposes
      Risk:       None, since all key material parts are defined as byte-arrays.
      Prevention: Covered by code review.
   MD_TLS_11.4_0310_ws:
      Reason:     Mapping an access pointer to a generic WorkSpace array
      Risk:       This might cause access problems on some platforms if elements are not aligned properly.
      Prevention: Covered by code review.
   MD_TLS_11.4_0310_parameter:
     Reason:      An API uses a uint8 array to forward data to the function, and depending on another parameter (e.g.
                  ParameterId) the data is interpreted in different ways.
     Risk:        This might cause access problems on some platforms if the data pointer is not set properly.
     Prevention:  Covered by code review.
   MD_TLS_11.4_0310_x:
      Reason:     others
      Risk:       not analysed
      Prevention: Covered by code review.
   MD_TLS_12.1_3393_3395:
      Reason:     Third party code is used here.
      Risk:       The compiler could handle elements in wrong order so that an intermediate result could be negative or
                  out of range of the variable.
      Prevention: Covered by Test, crypto operation will fail.
   MD_TLS_12.4_3415:
      Reason:     A concatenation of conditions may break without checking the later conditions if a previous one
                  already failed.
      Risk:       The concatenation of conditions might have a different effect as expected.
      Prevention: Covered by code review.
   MD_TLS_13.7_3355_3358:
      Reason:     This condition is always true only in case Det is enabled. To protect code against stack corruption
                  in case Det is disabled this if condition is added.
      Risk:       No risk.
      Prevention: Covered by code review.
   MD_TLS_13.7_3356:
      Reason:     This condition is always false only for certain configurations.
      Risk:       No risk.
      Prevention: Covered by code review.
   MD_TLS_16.7_3673:
     Reason:      Pointer could be const, but API has to be AUTOSAR-conform
     Risk:        Function could try to change the element the pointer points to, but this should not be allowed.
     Prevention:  Covered by code review.
   MD_TLS_19.15_0883:
     Reason:     QA-C MISRA checker does not recognize the protection agains multiple header include correctly.
     Risk:       No Risk.
     Prevention: Covered by code review.
   MD_TLS_21.1_3689:
      Reason:     Wraparound past zero or index out of bounds prevented by complex mechanism.
      Risk:       No risk.
      Prevention: Covered by code review.

   MD_TLS_3206:
      Reason:     API parameter is only used in some configurations.
      Risk:       There is no risk.
      Prevention: Covered by code review.
   MD_TLS_3305_sock:
     Reason:     The different socket address types do all base on the same stucture, the allignment should always be
                 the same so that mapping from one type to an other one should not be a problem.
     Risk:       Only casting between different variants of the socket type is allowed.
     Prevention: Covered by code review.
   MD_TLS_3305_param:
     Reason:     The API transfers parameters of different types but points to them using an uint8 pointer and a
                 parameter ID. The parameter type is specified for each parameter ID, so no false alignments should
                 occur.
     Risk:       Parameters could be casted to the wrong type accidently.
     Prevention: Covered by code review.
   MD_TLS_3305_struct:
     Reason:     TcpIp protocol headers are mapped to data arrays. The arrays should usually be aligned correctly.
     Risk:       Arrays could be misaligned accidently.
     Prevention: Covered by code review.
   MD_TLS_3305_copy:
      Reason:     The copy function uses a different base type as the local element.
      Risk:       The copy function might cause access problems on some platforms if elements are not aligned properly.
      Prevention: Covered by code review.
   MD_TLS_3305_ws:
      Reason:     Mapping an access pointer to a generic WorkSpace array
      Risk:       This might cause access problems on some platforms if elements are not aligned properly.
      Prevention: Covered by code review.

   TLS-specific deviations for single occurences:
   MD_TLS_9.1_3347_a:
      Reason:     The variable ExtraLenIv is always initialized properly. A few lines above the TLS protocol version is
                  checked, and for each version the variable ExtraLenIv is set, the default case (no valid protocol
                  version) will never be reached.
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TLS_9.1_3347_b:
      Reason:     The variable IvPtr is always initialized properly. A few lines above the TLS protocol version is
                  checked, and for each version the variable IvPtr is set, the default case (no valid protocol
                  version) will never be reached.
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TLS_9.1_3353_a:
      Reason:     The variable InclCertPtr is always initialized properly within the context of
                  'if((StatusDescr.RespFoundElements & TLS_STATUS_ELE_CERTS) != 0)'
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TLS_9.1_3354_a:
      Reason:     The array MacData[] is always initialized properly. At the place the function accesses the array
                  there is always a  MAC active and therefore is filled above.
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TLS_9.1_3354_b:
      Reason:     CombinedLabel_Array is always filled with data since both seed and label are never empty.
      Risk:       Both input arrays (seed and label) are empty.
      Prevention: Covered by code review.
   MD_TLS_9.1_3354_c:
      Reason:     The array IvBlock[] is always initialized for TLS1.2
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TLS_9.1_3354_d:
      Reason:     The array MacData[] is always initialized if a MAC is used.
      Risk:       The init sequence for the variable might change after a code change / rework.
      Prevention: Covered by code review.
   MD_TLS_TCP_14.6_0771_a:
      Reason:     The break statement is used multiple (four) times, two times to stop the function Tls_RxIndication
                  from trying to decode further or invalid message parts.
      Risk:       Unstructured code should be avoided.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TLS_TCP_14.6_0771_b:
      Reason:     The break statement is used two times to stop the function Tls_RxIndication
                  from looking for further buffers since a matching one was found.
      Risk:       Unstructured code should be avoided.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TLS_TCP_14.6_0771_c:
      Reason:     The break statement is used two times to stop the function Tls_VHs_RxServerHello
                  from further processing invalid elements.
      Risk:       Unstructured code should be avoided.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TLS_21.1_3372_a:
      Reason:     There won't be a wraparound! Only the padding is removed.
      Risk:       None. LocDecryptedLen will never be '0'.
      Prevention: Covered by code review.

*/

/**********************************************************************************************************************
 *  END OF FILE: Tls.c
 *********************************************************************************************************************/
