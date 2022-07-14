/* This header file contains some extracts from some asn.1 modules that are
   important for the library/es.
   These OIDs are mainly used inside the library to detect erroneous inputs and
   in order to produce correct outputs, that are matching currently used
   standards. */

#if !defined (CRYPTO_ESLib_ASN_1_H)
# define CRYPTO_ESLib_ASN_1_H

/* ansi_x9_62 */
#define OID_ansi_x9_62                 1*40+2,       /* iso(1) mmber-body(2) */ \
                                       0x86, 0x48,   /* us(840)              */ \
                                       0xce, 0x3d     /* 10045                */
#define LengthOfOID_ansi_x9_62         0x05

/* ansi_x9_62_fieldType_prime                   fieldType (1) GfP(1) */
#define OID_ansi_x9_62_fieldType_prime OID_ansi_x9_62, 0x01,  0x01
#define LengthOfOID_ansi_x9_62_fieldType_prime (2+LengthOfOID_ansi_x9_62)

/* Gf2 OIDs are only added for completeness. They will be supported by the
  library modules in future versions. */

/* ansi_x9_62_fieldType_char2                   fieldType (1) Gf2(2) */
#define OID_ansi_x9_62_fieldType_char2 OID_ansi_x9_62, 0x01,  0x02
#define LengthOfOID_ansi_x9_62_fieldType_char2 (2+LengthOfOID_ansi_x9_62)
/* ansi_x9_62_fieldType_char2gn                                    Basis_gn (1) */
#define OID_ansi_x9_62_fieldType_char2gn OID_ansi_x9_62_fieldType_char2, 0x01
#define LengthOfOID_ansi_x9_62_fieldType_char2gn (1+LengthOfOID_ansi_x9_62_fieldType_char2)
/* ansi_x9_62_fieldType_char2tp                                    Basis_tp (2) */
#define OID_ansi_x9_62_fieldType_char2tp OID_ansi_x9_62_fieldType_char2, 0x02
#define LengthOfOID_ansi_x9_62_fieldType_char2tp (1+LengthOfOID_ansi_x9_62_fieldType_char2)
/* ansi_x9_62_fieldType_char2pp                                    Basis_pp (3) */
#define OID_ansi_x9_62_fieldType_char2pp OID_ansi_x9_62_fieldType_char2, 0x03
#define LengthOfOID_ansi_x9_62_fieldType_char2pp (1+LengthOfOID_ansi_x9_62_fieldType_char2)


#define asn1Tag_INTEGER             2

#define asn1Tag_OBJECT_IDENTIFIER   6

#define asn1Tag_OCTET_STRING        4

#define asn1Tag_BIT_STRING          3

#define asn1Tag_COMPOUND          0x20

#define asn1Tag_SEQUENCE           ( 0x10 | asn1Tag_COMPOUND )


/* Block type for private key operation - uses 0x00 as padding (not recommended) */
#define ASN1_PADDING_BLOCK_TYPE_PRIVATE_ZERO    0x00u
/* Block type for private key operation - uses 0xFF as padding  (recommended) */
#define ASN1_PADDING_BLOCK_TYPE_PRIVATE         0x01u
/* Block type for public key operation */
#define ASN1_PADDING_BLOCK_TYPE_PUBLIC          0x02u

#define ASN1_MINIMAL_PADDING_LENGTH              8

/* tlvHeader_len + minSizeOfRandomBytes */
#define ASN1_SIZEOF_MINIMAL_PADDING              (3 + ASN1_MINIMAL_PADDING_LENGTH)

/* hash_len + tlvHeader_len + sizeof(byte) + digestInfo_len */
#define ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH (20 + 3 + 15)
#define ASN1_SIZEOF_SHA256_PARAMETERS_WITH_FIXED_LENGTH (32 + 3 + 19)

#define ASN1_MINIMAL_SIGVER_KEY_LENGTH           (ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH + ASN1_MINIMAL_PADDING_LENGTH)

/* DigestInfo algorithmIdentifier */
#define ASN1_DIGESTINFO_SHA1      {0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14}
#define ASN1_DIGESTINFO_SHA256    {0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20}
#define ASN1_DIGESTINFO_RIPEMD160 {0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x24, 0x03, 0x02, 0x01, 0x05, 0x00, 0x04, 0x14}


#endif /* CRYPTO_ESLib_ASN_1_H */
