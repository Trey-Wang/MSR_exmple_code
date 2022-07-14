/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  Crc.c
 *        \brief  Cyclic redundancy check header file
 *
 *      \details  Crc module is used to calculate cyclic codes, which encode messages by adding a fixed-length check
 *                value, for the purpose of error detection. Data blocks get a short CRC value attached, which is
 *                based on the remainder of a polynomial division of their contents. Upon retrieval of block data,
 *                the calculation is repeated and the calculated value is compared to the attached CRC value in order
 *                to ensure data integrity.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Crc.h"

/**********************************************************************************************************************
 *  VERSION CHECK  \trace SPEC-24723
 *********************************************************************************************************************/
#if(   (CRC_SW_MAJOR_VERSION != (5u) ) \
    || (CRC_SW_MINOR_VERSION != (4u) ))
# error "Version numbers of Crc.c and Crc.h are inconsistent!"
#endif

#if(   (CRC_CFG_MAJOR_VERSION != (3u) ) \
    || (CRC_CFG_MINOR_VERSION != (1u) ))
# error "Version numbers of Crc.c and Crc_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**** API Defines *****************************************************************************************************/

/* CRC8 polynomial */
#define CRC_INITIAL_VALUE8          (0xFFu)
#define CRC_POLYNOMIAL_8            (0x1Du)
#define CRC_FINAL_XOR_CRC8          (0xFFu)

/* CRC8 H2F polynomial */
#define CRC_INITIAL_VALUE8H2F       (0xFFu)
#define CRC_POLYNOMIAL_8H2F         (0x2Fu)
#define CRC_FINAL_XOR_CRC8H2F       (0xFFu)

/* CRC16 polynomial */
#define CRC_INITIAL_VALUE16         (0xFFFFu)
#define CRC_POLYNOMIAL_16           (0x1021u)

/* CRC32 polynomial */
#define CRC_INITIAL_VALUE32         (0xFFFFFFFFuL)
#define CRC_POLYNOMIAL_32           (0x04C11DB7uL)
#define CRC_POLYNOMIAL_32_REFLECT   (0xEDB88320uL)
#define CRC_FINAL_XOR_CRC32         (0xFFFFFFFFuL)

/* CRC32P4 polynomial */
#define CRC_INITIAL_VALUE32P4       (0xFFFFFFFFuL)
#define CRC_POLYNOMIAL_32P4         (0xF4ACFB13uL)
#define CRC_POLYNOMIAL_32P4_REFLECT (0xC8DF352FuL)
#define CRC_FINAL_XOR_CRC32P4       (0xFFFFFFFFuL)

/* CRC64 polynomial */
#define CRC_INITIAL_VALUE64         (0xFFFFFFFFFFFFFFFFuLL)
#define CRC_POLYNOMIAL_64           (0x42F0E1EBA9EA3693uLL)
#define CRC_POLYNOMIAL_64_REFLECT   (0xC96C5795D7870F42uLL)
#define CRC_FINAL_XOR_CRC64         (0xFFFFFFFFFFFFFFFFuLL)

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CRC_LOCAL) /* COV_CRC_COMPATIBILITY */
# define CRC_LOCAL static
#endif

#if !defined (CRC_LOCAL_INLINE) /* COV_CRC_COMPATIBILITY */
# define CRC_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

#if (CRC_8_MODE == CRC_8_TABLE)
/**********************************************************************************************************************
 *  CRC 8 LOOK-UP TABLE
 *********************************************************************************************************************/
# define CRC_START_SEC_CONST_8
# include "MemMap.h"          /* PRQA S 5087 */ /* MD_MSR_19.1 */

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
 *
 *
 */

CRC_LOCAL CONST(uint8, CRC_CONST) Crc_Table8[256] = /* PRQA S 3218 */ /* MD_CRC_8.7_FileScopeStatic */
{
    0x00u, 0x1du, 0x3au, 0x27u,
    0x74u, 0x69u, 0x4eu, 0x53u,
    0xe8u, 0xf5u, 0xd2u, 0xcfu,
    0x9cu, 0x81u, 0xa6u, 0xbbu,
    0xcdu, 0xd0u, 0xf7u, 0xeau,
    0xb9u, 0xa4u, 0x83u, 0x9eu,
    0x25u, 0x38u, 0x1fu, 0x02u,
    0x51u, 0x4cu, 0x6bu, 0x76u,
    0x87u, 0x9au, 0xbdu, 0xa0u,
    0xf3u, 0xeeu, 0xc9u, 0xd4u,
    0x6fu, 0x72u, 0x55u, 0x48u,
    0x1bu, 0x06u, 0x21u, 0x3cu,
    0x4au, 0x57u, 0x70u, 0x6du,
    0x3eu, 0x23u, 0x04u, 0x19u,
    0xa2u, 0xbfu, 0x98u, 0x85u,
    0xd6u, 0xcbu, 0xecu, 0xf1u,
    0x13u, 0x0eu, 0x29u, 0x34u,
    0x67u, 0x7au, 0x5du, 0x40u,
    0xfbu, 0xe6u, 0xc1u, 0xdcu,
    0x8fu, 0x92u, 0xb5u, 0xa8u,
    0xdeu, 0xc3u, 0xe4u, 0xf9u,
    0xaau, 0xb7u, 0x90u, 0x8du,
    0x36u, 0x2bu, 0x0cu, 0x11u,
    0x42u, 0x5fu, 0x78u, 0x65u,
    0x94u, 0x89u, 0xaeu, 0xb3u,
    0xe0u, 0xfdu, 0xdau, 0xc7u,
    0x7cu, 0x61u, 0x46u, 0x5bu,
    0x08u, 0x15u, 0x32u, 0x2fu,
    0x59u, 0x44u, 0x63u, 0x7eu,
    0x2du, 0x30u, 0x17u, 0x0au,
    0xb1u, 0xacu, 0x8bu, 0x96u,
    0xc5u, 0xd8u, 0xffu, 0xe2u,
    0x26u, 0x3bu, 0x1cu, 0x01u,
    0x52u, 0x4fu, 0x68u, 0x75u,
    0xceu, 0xd3u, 0xf4u, 0xe9u,
    0xbau, 0xa7u, 0x80u, 0x9du,
    0xebu, 0xf6u, 0xd1u, 0xccu,
    0x9fu, 0x82u, 0xa5u, 0xb8u,
    0x03u, 0x1eu, 0x39u, 0x24u,
    0x77u, 0x6au, 0x4du, 0x50u,
    0xa1u, 0xbcu, 0x9bu, 0x86u,
    0xd5u, 0xc8u, 0xefu, 0xf2u,
    0x49u, 0x54u, 0x73u, 0x6eu,
    0x3du, 0x20u, 0x07u, 0x1au,
    0x6cu, 0x71u, 0x56u, 0x4bu,
    0x18u, 0x05u, 0x22u, 0x3fu,
    0x84u, 0x99u, 0xbeu, 0xa3u,
    0xf0u, 0xedu, 0xcau, 0xd7u,
    0x35u, 0x28u, 0x0fu, 0x12u,
    0x41u, 0x5cu, 0x7bu, 0x66u,
    0xddu, 0xc0u, 0xe7u, 0xfau,
    0xa9u, 0xb4u, 0x93u, 0x8eu,
    0xf8u, 0xe5u, 0xc2u, 0xdfu,
    0x8cu, 0x91u, 0xb6u, 0xabu,
    0x10u, 0x0du, 0x2au, 0x37u,
    0x64u, 0x79u, 0x5eu, 0x43u,
    0xb2u, 0xafu, 0x88u, 0x95u,
    0xc6u, 0xdbu, 0xfcu, 0xe1u,
    0x5au, 0x47u, 0x60u, 0x7du,
    0x2eu, 0x33u, 0x14u, 0x09u,
    0x7fu, 0x62u, 0x45u, 0x58u,
    0x0bu, 0x16u, 0x31u, 0x2cu,
    0x97u, 0x8au, 0xadu, 0xb0u,
    0xe3u, 0xfeu, 0xd9u, 0xc4u
};

# define CRC_STOP_SEC_CONST_8
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CRC_8_MODE == CRC_8_TABLE */

#if (CRC_8H2F_MODE == CRC_8H2F_TABLE)
/**********************************************************************************************************************
 *  CRC 8 H2F LOOK-UP TABLE
 *********************************************************************************************************************/
# define CRC_START_SEC_CONST_8
# include "MemMap.h"          /* PRQA S 5087 */ /* MD_MSR_19.1 */

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
 *
 *
 */

CRC_LOCAL CONST(uint8, CRC_CONST) Crc_Table8H2F[256] = /* PRQA S 3218 */ /* MD_CRC_8.7_FileScopeStatic */
{
    0x00u,0x2fu,0x5eu,0x71u,0xbcu,0x93u,0xe2u,0xcdu,0x57u,0x78u,0x09u,0x26u,0xebu,
    0xc4u,0xb5u,0x9au,0xaeu,0x81u,0xf0u,0xdfu,0x12u,0x3du,0x4cu,0x63u,0xf9u,0xd6u,
    0xa7u,0x88u,0x45u,0x6au,0x1bu,0x34u,0x73u,0x5cu,0x2du,0x02u,0xcfu,0xe0u,0x91u,
    0xbeu,0x24u,0x0bu,0x7au,0x55u,0x98u,0xb7u,0xc6u,0xe9u,0xddu,0xf2u,0x83u,0xacu,
    0x61u,0x4eu,0x3fu,0x10u,0x8au,0xa5u,0xd4u,0xfbu,0x36u,0x19u,0x68u,0x47u,0xe6u,
    0xc9u,0xb8u,0x97u,0x5au,0x75u,0x04u,0x2bu,0xb1u,0x9eu,0xefu,0xc0u,0x0du,0x22u,
    0x53u,0x7cu,0x48u,0x67u,0x16u,0x39u,0xf4u,0xdbu,0xaau,0x85u,0x1fu,0x30u,0x41u,
    0x6eu,0xa3u,0x8cu,0xfdu,0xd2u,0x95u,0xbau,0xcbu,0xe4u,0x29u,0x06u,0x77u,0x58u,
    0xc2u,0xedu,0x9cu,0xb3u,0x7eu,0x51u,0x20u,0x0fu,0x3bu,0x14u,0x65u,0x4au,0x87u,
    0xa8u,0xd9u,0xf6u,0x6cu,0x43u,0x32u,0x1du,0xd0u,0xffu,0x8eu,0xa1u,0xe3u,0xccu,
    0xbdu,0x92u,0x5fu,0x70u,0x01u,0x2eu,0xb4u,0x9bu,0xeau,0xc5u,0x08u,0x27u,0x56u,
    0x79u,0x4du,0x62u,0x13u,0x3cu,0xf1u,0xdeu,0xafu,0x80u,0x1au,0x35u,0x44u,0x6bu,
    0xa6u,0x89u,0xf8u,0xd7u,0x90u,0xbfu,0xceu,0xe1u,0x2cu,0x03u,0x72u,0x5du,0xc7u,
    0xe8u,0x99u,0xb6u,0x7bu,0x54u,0x25u,0x0au,0x3eu,0x11u,0x60u,0x4fu,0x82u,0xadu,
    0xdcu,0xf3u,0x69u,0x46u,0x37u,0x18u,0xd5u,0xfau,0x8bu,0xa4u,0x05u,0x2au,0x5bu,
    0x74u,0xb9u,0x96u,0xe7u,0xc8u,0x52u,0x7du,0x0cu,0x23u,0xeeu,0xc1u,0xb0u,0x9fu,
    0xabu,0x84u,0xf5u,0xdau,0x17u,0x38u,0x49u,0x66u,0xfcu,0xd3u,0xa2u,0x8du,0x40u,
    0x6fu,0x1eu,0x31u,0x76u,0x59u,0x28u,0x07u,0xcau,0xe5u,0x94u,0xbbu,0x21u,0x0eu,
    0x7fu,0x50u,0x9du,0xb2u,0xc3u,0xecu,0xd8u,0xf7u,0x86u,0xa9u,0x64u,0x4bu,0x3au,
    0x15u,0x8fu,0xa0u,0xd1u,0xfeu,0x33u,0x1cu,0x6du,0x42u
};

# define CRC_STOP_SEC_CONST_8
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CRC_8H2F_MODE == CRC_8H2F_TABLE */

#if (CRC_16_MODE == CRC_16_TABLE)
/**********************************************************************************************************************
 *  CRC 16 LOOK-UP TABLE
 *********************************************************************************************************************/
# define CRC_START_SEC_CONST_16
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

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
 *
 *
 */

CRC_LOCAL CONST(uint16, CRC_CONST) Crc_Table16[256] = /* PRQA S 3218 */ /* MD_CRC_8.7_FileScopeStatic */
{
    0x0000u, 0x1021u, 0x2042u, 0x3063u, 0x4084u, 0x50a5u, 0x60c6u, 0x70e7u,
    0x8108u, 0x9129u, 0xa14au, 0xb16bu, 0xc18cu, 0xd1adu, 0xe1ceu, 0xf1efu,
    0x1231u, 0x0210u, 0x3273u, 0x2252u, 0x52b5u, 0x4294u, 0x72f7u, 0x62d6u,
    0x9339u, 0x8318u, 0xb37bu, 0xa35au, 0xd3bdu, 0xc39cu, 0xf3ffu, 0xe3deu,
    0x2462u, 0x3443u, 0x0420u, 0x1401u, 0x64e6u, 0x74c7u, 0x44a4u, 0x5485u,
    0xa56au, 0xb54bu, 0x8528u, 0x9509u, 0xe5eeu, 0xf5cfu, 0xc5acu, 0xd58du,
    0x3653u, 0x2672u, 0x1611u, 0x0630u, 0x76d7u, 0x66f6u, 0x5695u, 0x46b4u,
    0xb75bu, 0xa77au, 0x9719u, 0x8738u, 0xf7dfu, 0xe7feu, 0xd79du, 0xc7bcu,
    0x48c4u, 0x58e5u, 0x6886u, 0x78a7u, 0x0840u, 0x1861u, 0x2802u, 0x3823u,
    0xc9ccu, 0xd9edu, 0xe98eu, 0xf9afu, 0x8948u, 0x9969u, 0xa90au, 0xb92bu,
    0x5af5u, 0x4ad4u, 0x7ab7u, 0x6a96u, 0x1a71u, 0x0a50u, 0x3a33u, 0x2a12u,
    0xdbfdu, 0xcbdcu, 0xfbbfu, 0xeb9eu, 0x9b79u, 0x8b58u, 0xbb3bu, 0xab1au,
    0x6ca6u, 0x7c87u, 0x4ce4u, 0x5cc5u, 0x2c22u, 0x3c03u, 0x0c60u, 0x1c41u,
    0xedaeu, 0xfd8fu, 0xcdecu, 0xddcdu, 0xad2au, 0xbd0bu, 0x8d68u, 0x9d49u,
    0x7e97u, 0x6eb6u, 0x5ed5u, 0x4ef4u, 0x3e13u, 0x2e32u, 0x1e51u, 0x0e70u,
    0xff9fu, 0xefbeu, 0xdfddu, 0xcffcu, 0xbf1bu, 0xaf3au, 0x9f59u, 0x8f78u,
    0x9188u, 0x81a9u, 0xb1cau, 0xa1ebu, 0xd10cu, 0xc12du, 0xf14eu, 0xe16fu,
    0x1080u, 0x00a1u, 0x30c2u, 0x20e3u, 0x5004u, 0x4025u, 0x7046u, 0x6067u,
    0x83b9u, 0x9398u, 0xa3fbu, 0xb3dau, 0xc33du, 0xd31cu, 0xe37fu, 0xf35eu,
    0x02b1u, 0x1290u, 0x22f3u, 0x32d2u, 0x4235u, 0x5214u, 0x6277u, 0x7256u,
    0xb5eau, 0xa5cbu, 0x95a8u, 0x8589u, 0xf56eu, 0xe54fu, 0xd52cu, 0xc50du,
    0x34e2u, 0x24c3u, 0x14a0u, 0x0481u, 0x7466u, 0x6447u, 0x5424u, 0x4405u,
    0xa7dbu, 0xb7fau, 0x8799u, 0x97b8u, 0xe75fu, 0xf77eu, 0xc71du, 0xd73cu,
    0x26d3u, 0x36f2u, 0x0691u, 0x16b0u, 0x6657u, 0x7676u, 0x4615u, 0x5634u,
    0xd94cu, 0xc96du, 0xf90eu, 0xe92fu, 0x99c8u, 0x89e9u, 0xb98au, 0xa9abu,
    0x5844u, 0x4865u, 0x7806u, 0x6827u, 0x18c0u, 0x08e1u, 0x3882u, 0x28a3u,
    0xcb7du, 0xdb5cu, 0xeb3fu, 0xfb1eu, 0x8bf9u, 0x9bd8u, 0xabbbu, 0xbb9au,
    0x4a75u, 0x5a54u, 0x6a37u, 0x7a16u, 0x0af1u, 0x1ad0u, 0x2ab3u, 0x3a92u,
    0xfd2eu, 0xed0fu, 0xdd6cu, 0xcd4du, 0xbdaau, 0xad8bu, 0x9de8u, 0x8dc9u,
    0x7c26u, 0x6c07u, 0x5c64u, 0x4c45u, 0x3ca2u, 0x2c83u, 0x1ce0u, 0x0cc1u,
    0xef1fu, 0xff3eu, 0xcf5du, 0xdf7cu, 0xaf9bu, 0xbfbau, 0x8fd9u, 0x9ff8u,
    0x6e17u, 0x7e36u, 0x4e55u, 0x5e74u, 0x2e93u, 0x3eb2u, 0x0ed1u, 0x1ef0u
};

# define CRC_STOP_SEC_CONST_16
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CRC_16_MODE == CRC_16_TABLE */

#if (CRC_32_MODE == CRC_32_TABLE)
/**********************************************************************************************************************
 *  CRC 32 LOOK-UP TABLE
 *********************************************************************************************************************/
# define CRC_START_SEC_CONST_32
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

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
 *
 *
 */

CRC_LOCAL CONST(uint32, CRC_CONST) Crc_Table32[256] = /* PRQA S 3218 */ /* MD_CRC_8.7_FileScopeStatic */
{
    0x00000000uL, 0x77073096uL, 0xEE0E612CuL, 0x990951BAuL,
    0x076DC419uL, 0x706AF48FuL, 0xE963A535uL, 0x9E6495A3uL,
    0x0EDB8832uL, 0x79DCB8A4uL, 0xE0D5E91EuL, 0x97D2D988uL,
    0x09B64C2BuL, 0x7EB17CBDuL, 0xE7B82D07uL, 0x90BF1D91uL,
    0x1DB71064uL, 0x6AB020F2uL, 0xF3B97148uL, 0x84BE41DEuL,
    0x1ADAD47DuL, 0x6DDDE4EBuL, 0xF4D4B551uL, 0x83D385C7uL,
    0x136C9856uL, 0x646BA8C0uL, 0xFD62F97AuL, 0x8A65C9ECuL,
    0x14015C4FuL, 0x63066CD9uL, 0xFA0F3D63uL, 0x8D080DF5uL,
    0x3B6E20C8uL, 0x4C69105EuL, 0xD56041E4uL, 0xA2677172uL,
    0x3C03E4D1uL, 0x4B04D447uL, 0xD20D85FDuL, 0xA50AB56BuL,
    0x35B5A8FAuL, 0x42B2986CuL, 0xDBBBC9D6uL, 0xACBCF940uL,
    0x32D86CE3uL, 0x45DF5C75uL, 0xDCD60DCFuL, 0xABD13D59uL,
    0x26D930ACuL, 0x51DE003AuL, 0xC8D75180uL, 0xBFD06116uL,
    0x21B4F4B5uL, 0x56B3C423uL, 0xCFBA9599uL, 0xB8BDA50FuL,
    0x2802B89EuL, 0x5F058808uL, 0xC60CD9B2uL, 0xB10BE924uL,
    0x2F6F7C87uL, 0x58684C11uL, 0xC1611DABuL, 0xB6662D3DuL,
    0x76DC4190uL, 0x01DB7106uL, 0x98D220BCuL, 0xEFD5102AuL,
    0x71B18589uL, 0x06B6B51FuL, 0x9FBFE4A5uL, 0xE8B8D433uL,
    0x7807C9A2uL, 0x0F00F934uL, 0x9609A88EuL, 0xE10E9818uL,
    0x7F6A0DBBuL, 0x086D3D2DuL, 0x91646C97uL, 0xE6635C01uL,
    0x6B6B51F4uL, 0x1C6C6162uL, 0x856530D8uL, 0xF262004EuL,
    0x6C0695EDuL, 0x1B01A57BuL, 0x8208F4C1uL, 0xF50FC457uL,
    0x65B0D9C6uL, 0x12B7E950uL, 0x8BBEB8EAuL, 0xFCB9887CuL,
    0x62DD1DDFuL, 0x15DA2D49uL, 0x8CD37CF3uL, 0xFBD44C65uL,
    0x4DB26158uL, 0x3AB551CEuL, 0xA3BC0074uL, 0xD4BB30E2uL,
    0x4ADFA541uL, 0x3DD895D7uL, 0xA4D1C46DuL, 0xD3D6F4FBuL,
    0x4369E96AuL, 0x346ED9FCuL, 0xAD678846uL, 0xDA60B8D0uL,
    0x44042D73uL, 0x33031DE5uL, 0xAA0A4C5FuL, 0xDD0D7CC9uL,
    0x5005713CuL, 0x270241AAuL, 0xBE0B1010uL, 0xC90C2086uL,
    0x5768B525uL, 0x206F85B3uL, 0xB966D409uL, 0xCE61E49FuL,
    0x5EDEF90EuL, 0x29D9C998uL, 0xB0D09822uL, 0xC7D7A8B4uL,
    0x59B33D17uL, 0x2EB40D81uL, 0xB7BD5C3BuL, 0xC0BA6CADuL,
    0xEDB88320uL, 0x9ABFB3B6uL, 0x03B6E20CuL, 0x74B1D29AuL,
    0xEAD54739uL, 0x9DD277AFuL, 0x04DB2615uL, 0x73DC1683uL,
    0xE3630B12uL, 0x94643B84uL, 0x0D6D6A3EuL, 0x7A6A5AA8uL,
    0xE40ECF0BuL, 0x9309FF9DuL, 0x0A00AE27uL, 0x7D079EB1uL,
    0xF00F9344uL, 0x8708A3D2uL, 0x1E01F268uL, 0x6906C2FEuL,
    0xF762575DuL, 0x806567CBuL, 0x196C3671uL, 0x6E6B06E7uL,
    0xFED41B76uL, 0x89D32BE0uL, 0x10DA7A5AuL, 0x67DD4ACCuL,
    0xF9B9DF6FuL, 0x8EBEEFF9uL, 0x17B7BE43uL, 0x60B08ED5uL,
    0xD6D6A3E8uL, 0xA1D1937EuL, 0x38D8C2C4uL, 0x4FDFF252uL,
    0xD1BB67F1uL, 0xA6BC5767uL, 0x3FB506DDuL, 0x48B2364BuL,
    0xD80D2BDAuL, 0xAF0A1B4CuL, 0x36034AF6uL, 0x41047A60uL,
    0xDF60EFC3uL, 0xA867DF55uL, 0x316E8EEFuL, 0x4669BE79uL,
    0xCB61B38CuL, 0xBC66831AuL, 0x256FD2A0uL, 0x5268E236uL,
    0xCC0C7795uL, 0xBB0B4703uL, 0x220216B9uL, 0x5505262FuL,
    0xC5BA3BBEuL, 0xB2BD0B28uL, 0x2BB45A92uL, 0x5CB36A04uL,
    0xC2D7FFA7uL, 0xB5D0CF31uL, 0x2CD99E8BuL, 0x5BDEAE1DuL,
    0x9B64C2B0uL, 0xEC63F226uL, 0x756AA39CuL, 0x026D930AuL,
    0x9C0906A9uL, 0xEB0E363FuL, 0x72076785uL, 0x05005713uL,
    0x95BF4A82uL, 0xE2B87A14uL, 0x7BB12BAEuL, 0x0CB61B38uL,
    0x92D28E9BuL, 0xE5D5BE0DuL, 0x7CDCEFB7uL, 0x0BDBDF21uL,
    0x86D3D2D4uL, 0xF1D4E242uL, 0x68DDB3F8uL, 0x1FDA836EuL,
    0x81BE16CDuL, 0xF6B9265BuL, 0x6FB077E1uL, 0x18B74777uL,
    0x88085AE6uL, 0xFF0F6A70uL, 0x66063BCAuL, 0x11010B5CuL,
    0x8F659EFFuL, 0xF862AE69uL, 0x616BFFD3uL, 0x166CCF45uL,
    0xA00AE278uL, 0xD70DD2EEuL, 0x4E048354uL, 0x3903B3C2uL,
    0xA7672661uL, 0xD06016F7uL, 0x4969474DuL, 0x3E6E77DBuL,
    0xAED16A4AuL, 0xD9D65ADCuL, 0x40DF0B66uL, 0x37D83BF0uL,
    0xA9BCAE53uL, 0xDEBB9EC5uL, 0x47B2CF7FuL, 0x30B5FFE9uL,
    0xBDBDF21CuL, 0xCABAC28AuL, 0x53B39330uL, 0x24B4A3A6uL,
    0xBAD03605uL, 0xCDD70693uL, 0x54DE5729uL, 0x23D967BFuL,
    0xB3667A2EuL, 0xC4614AB8uL, 0x5D681B02uL, 0x2A6F2B94uL,
    0xB40BBE37uL, 0xC30C8EA1uL, 0x5A05DF1BuL, 0x2D02EF8DuL
};

# define CRC_STOP_SEC_CONST_32
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CRC_32_MODE == CRC_32_TABLE */

#if (CRC_32P4_MODE == CRC_32P4_TABLE)
/**********************************************************************************************************************
 *  CRC 32P4 LOOK-UP TABLE
 *********************************************************************************************************************/
# define CRC_START_SEC_CONST_32
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

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
 *
 *
 */

CRC_LOCAL CONST(uint32, CRC_CONST) Crc_Table32P4[256] = /* PRQA S 3218 */ /* MD_CRC_8.7_FileScopeStatic */
{
    0x00000000uL, 0x30850FF5uL, 0x610A1FEAuL, 0x518F101FuL, 0xC2143FD4uL, 0xF2913021uL, 0xA31E203EuL, 0x939B2FCBuL,
    0x159615F7uL, 0x25131A02uL, 0x749C0A1DuL, 0x441905E8uL, 0xD7822A23uL, 0xE70725D6uL, 0xB68835C9uL, 0x860D3A3CuL,
    0x2B2C2BEEuL, 0x1BA9241BuL, 0x4A263404uL, 0x7AA33BF1uL, 0xE938143AuL, 0xD9BD1BCFuL, 0x88320BD0uL, 0xB8B70425uL,
    0x3EBA3E19uL, 0x0E3F31ECuL, 0x5FB021F3uL, 0x6F352E06uL, 0xFCAE01CDuL, 0xCC2B0E38uL, 0x9DA41E27uL, 0xAD2111D2uL,
    0x565857DCuL, 0x66DD5829uL, 0x37524836uL, 0x07D747C3uL, 0x944C6808uL, 0xA4C967FDuL, 0xF54677E2uL, 0xC5C37817uL,
    0x43CE422BuL, 0x734B4DDEuL, 0x22C45DC1uL, 0x12415234uL, 0x81DA7DFFuL, 0xB15F720AuL, 0xE0D06215uL, 0xD0556DE0uL,
    0x7D747C32uL, 0x4DF173C7uL, 0x1C7E63D8uL, 0x2CFB6C2DuL, 0xBF6043E6uL, 0x8FE54C13uL, 0xDE6A5C0CuL, 0xEEEF53F9uL,
    0x68E269C5uL, 0x58676630uL, 0x09E8762FuL, 0x396D79DAuL, 0xAAF65611uL, 0x9A7359E4uL, 0xCBFC49FBuL, 0xFB79460EuL,
    0xACB0AFB8uL, 0x9C35A04DuL, 0xCDBAB052uL, 0xFD3FBFA7uL, 0x6EA4906CuL, 0x5E219F99uL, 0x0FAE8F86uL, 0x3F2B8073uL,
    0xB926BA4FuL, 0x89A3B5BAuL, 0xD82CA5A5uL, 0xE8A9AA50uL, 0x7B32859BuL, 0x4BB78A6EuL, 0x1A389A71uL, 0x2ABD9584uL,
    0x879C8456uL, 0xB7198BA3uL, 0xE6969BBCuL, 0xD6139449uL, 0x4588BB82uL, 0x750DB477uL, 0x2482A468uL, 0x1407AB9DuL,
    0x920A91A1uL, 0xA28F9E54uL, 0xF3008E4BuL, 0xC38581BEuL, 0x501EAE75uL, 0x609BA180uL, 0x3114B19FuL, 0x0191BE6AuL,
    0xFAE8F864uL, 0xCA6DF791uL, 0x9BE2E78EuL, 0xAB67E87BuL, 0x38FCC7B0uL, 0x0879C845uL, 0x59F6D85AuL, 0x6973D7AFuL,
    0xEF7EED93uL, 0xDFFBE266uL, 0x8E74F279uL, 0xBEF1FD8CuL, 0x2D6AD247uL, 0x1DEFDDB2uL, 0x4C60CDADuL, 0x7CE5C258uL,
    0xD1C4D38AuL, 0xE141DC7FuL, 0xB0CECC60uL, 0x804BC395uL, 0x13D0EC5EuL, 0x2355E3ABuL, 0x72DAF3B4uL, 0x425FFC41uL,
    0xC452C67DuL, 0xF4D7C988uL, 0xA558D997uL, 0x95DDD662uL, 0x0646F9A9uL, 0x36C3F65CuL, 0x674CE643uL, 0x57C9E9B6uL,
    0xC8DF352FuL, 0xF85A3ADAuL, 0xA9D52AC5uL, 0x99502530uL, 0x0ACB0AFBuL, 0x3A4E050EuL, 0x6BC11511uL, 0x5B441AE4uL,
    0xDD4920D8uL, 0xEDCC2F2DuL, 0xBC433F32uL, 0x8CC630C7uL, 0x1F5D1F0CuL, 0x2FD810F9uL, 0x7E5700E6uL, 0x4ED20F13uL,
    0xE3F31EC1uL, 0xD3761134uL, 0x82F9012BuL, 0xB27C0EDEuL, 0x21E72115uL, 0x11622EE0uL, 0x40ED3EFFuL, 0x7068310AuL,
    0xF6650B36uL, 0xC6E004C3uL, 0x976F14DCuL, 0xA7EA1B29uL, 0x347134E2uL, 0x04F43B17uL, 0x557B2B08uL, 0x65FE24FDuL,
    0x9E8762F3uL, 0xAE026D06uL, 0xFF8D7D19uL, 0xCF0872ECuL, 0x5C935D27uL, 0x6C1652D2uL, 0x3D9942CDuL, 0x0D1C4D38uL,
    0x8B117704uL, 0xBB9478F1uL, 0xEA1B68EEuL, 0xDA9E671BuL, 0x490548D0uL, 0x79804725uL, 0x280F573AuL, 0x188A58CFuL,
    0xB5AB491DuL, 0x852E46E8uL, 0xD4A156F7uL, 0xE4245902uL, 0x77BF76C9uL, 0x473A793CuL, 0x16B56923uL, 0x263066D6uL,
    0xA03D5CEAuL, 0x90B8531FuL, 0xC1374300uL, 0xF1B24CF5uL, 0x6229633EuL, 0x52AC6CCBuL, 0x03237CD4uL, 0x33A67321uL,
    0x646F9A97uL, 0x54EA9562uL, 0x0565857DuL, 0x35E08A88uL, 0xA67BA543uL, 0x96FEAAB6uL, 0xC771BAA9uL, 0xF7F4B55CuL,
    0x71F98F60uL, 0x417C8095uL, 0x10F3908AuL, 0x20769F7FuL, 0xB3EDB0B4uL, 0x8368BF41uL, 0xD2E7AF5EuL, 0xE262A0ABuL,
    0x4F43B179uL, 0x7FC6BE8CuL, 0x2E49AE93uL, 0x1ECCA166uL, 0x8D578EADuL, 0xBDD28158uL, 0xEC5D9147uL, 0xDCD89EB2uL,
    0x5AD5A48EuL, 0x6A50AB7BuL, 0x3BDFBB64uL, 0x0B5AB491uL, 0x98C19B5AuL, 0xA84494AFuL, 0xF9CB84B0uL, 0xC94E8B45uL,
    0x3237CD4BuL, 0x02B2C2BEuL, 0x533DD2A1uL, 0x63B8DD54uL, 0xF023F29FuL, 0xC0A6FD6AuL, 0x9129ED75uL, 0xA1ACE280uL,
    0x27A1D8BCuL, 0x1724D749uL, 0x46ABC756uL, 0x762EC8A3uL, 0xE5B5E768uL, 0xD530E89DuL, 0x84BFF882uL, 0xB43AF777uL,
    0x191BE6A5uL, 0x299EE950uL, 0x7811F94FuL, 0x4894F6BAuL, 0xDB0FD971uL, 0xEB8AD684uL, 0xBA05C69BuL, 0x8A80C96EuL,
    0x0C8DF352uL, 0x3C08FCA7uL, 0x6D87ECB8uL, 0x5D02E34DuL, 0xCE99CC86uL, 0xFE1CC373uL, 0xAF93D36CuL, 0x9F16DC99uL
};

# define CRC_STOP_SEC_CONST_32
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CRC_32P4_MODE == CRC_32P4_TABLE */

#if (CRC_64_MODE == CRC_64_TABLE)
/**********************************************************************************************************************
 *  CRC 64 LOOK-UP TABLE
 *********************************************************************************************************************/
# define CRC_START_SEC_CONST_64
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

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
 *
 *
 */

CRC_LOCAL CONST(uint64, CRC_CONST) Crc_Table64[256] = /* PRQA S 3218 */ /* MD_CRC_8.7_FileScopeStatic */
{
    0x0uLL, 0xB32E4CBE03A75F6FuLL, 0xF4843657A840A05BuLL, 0x47AA7AE9ABE7FF34uLL, 0x7BD0C384FF8F5E33uLL, 0xC8FE8F3AFC28015CuLL, 0x8F54F5D357CFFE68uLL, 0x3C7AB96D5468A107uLL,
    0xF7A18709FF1EBC66uLL, 0x448FCBB7FCB9E309uLL, 0x325B15E575E1C3DuLL, 0xB00BFDE054F94352uLL, 0x8C71448D0091E255uLL, 0x3F5F08330336BD3AuLL, 0x78F572DAA8D1420EuLL, 0xCBDB3E64AB761D61uLL,
    0x7D9BA13851336649uLL, 0xCEB5ED8652943926uLL, 0x891F976FF973C612uLL, 0x3A31DBD1FAD4997DuLL, 0x64B62BCAEBC387AuLL, 0xB5652E02AD1B6715uLL, 0xF2CF54EB06FC9821uLL, 0x41E11855055BC74EuLL,
    0x8A3A2631AE2DDA2FuLL, 0x39146A8FAD8A8540uLL, 0x7EBE1066066D7A74uLL, 0xCD905CD805CA251BuLL, 0xF1EAE5B551A2841CuLL, 0x42C4A90B5205DB73uLL, 0x56ED3E2F9E22447uLL, 0xB6409F5CFA457B28uLL,
    0xFB374270A266CC92uLL, 0x48190ECEA1C193FDuLL, 0xFB374270A266CC9uLL, 0xBC9D3899098133A6uLL, 0x80E781F45DE992A1uLL, 0x33C9CD4A5E4ECDCEuLL, 0x7463B7A3F5A932FAuLL, 0xC74DFB1DF60E6D95uLL,
    0xC96C5795D7870F4uLL, 0xBFB889C75EDF2F9BuLL, 0xF812F32EF538D0AFuLL, 0x4B3CBF90F69F8FC0uLL, 0x774606FDA2F72EC7uLL, 0xC4684A43A15071A8uLL, 0x83C230AA0AB78E9CuLL, 0x30EC7C140910D1F3uLL,
    0x86ACE348F355AADBuLL, 0x3582AFF6F0F2F5B4uLL, 0x7228D51F5B150A80uLL, 0xC10699A158B255EFuLL, 0xFD7C20CC0CDAF4E8uLL, 0x4E526C720F7DAB87uLL, 0x9F8169BA49A54B3uLL, 0xBAD65A25A73D0BDCuLL,
    0x710D64410C4B16BDuLL, 0xC22328FF0FEC49D2uLL, 0x85895216A40BB6E6uLL, 0x36A71EA8A7ACE989uLL, 0xADDA7C5F3C4488EuLL, 0xB9F3EB7BF06317E1uLL, 0xFE5991925B84E8D5uLL, 0x4D77DD2C5823B7BAuLL,
    0x64B62BCAEBC387A1uLL, 0xD7986774E864D8CEuLL, 0x90321D9D438327FAuLL, 0x231C512340247895uLL, 0x1F66E84E144CD992uLL, 0xAC48A4F017EB86FDuLL, 0xEBE2DE19BC0C79C9uLL, 0x58CC92A7BFAB26A6uLL,
    0x9317ACC314DD3BC7uLL, 0x2039E07D177A64A8uLL, 0x67939A94BC9D9B9CuLL, 0xD4BDD62ABF3AC4F3uLL, 0xE8C76F47EB5265F4uLL, 0x5BE923F9E8F53A9BuLL, 0x1C4359104312C5AFuLL, 0xAF6D15AE40B59AC0uLL,
    0x192D8AF2BAF0E1E8uLL, 0xAA03C64CB957BE87uLL, 0xEDA9BCA512B041B3uLL, 0x5E87F01B11171EDCuLL, 0x62FD4976457FBFDBuLL, 0xD1D305C846D8E0B4uLL, 0x96797F21ED3F1F80uLL, 0x2557339FEE9840EFuLL,
    0xEE8C0DFB45EE5D8EuLL, 0x5DA24145464902E1uLL, 0x1A083BACEDAEFDD5uLL, 0xA9267712EE09A2BAuLL, 0x955CCE7FBA6103BDuLL, 0x267282C1B9C65CD2uLL, 0x61D8F8281221A3E6uLL, 0xD2F6B4961186FC89uLL,
    0x9F8169BA49A54B33uLL, 0x2CAF25044A02145CuLL, 0x6B055FEDE1E5EB68uLL, 0xD82B1353E242B407uLL, 0xE451AA3EB62A1500uLL, 0x577FE680B58D4A6FuLL, 0x10D59C691E6AB55BuLL, 0xA3FBD0D71DCDEA34uLL,
    0x6820EEB3B6BBF755uLL, 0xDB0EA20DB51CA83AuLL, 0x9CA4D8E41EFB570EuLL, 0x2F8A945A1D5C0861uLL, 0x13F02D374934A966uLL, 0xA0DE61894A93F609uLL, 0xE7741B60E174093DuLL, 0x545A57DEE2D35652uLL,
    0xE21AC88218962D7AuLL, 0x5134843C1B317215uLL, 0x169EFED5B0D68D21uLL, 0xA5B0B26BB371D24EuLL, 0x99CA0B06E7197349uLL, 0x2AE447B8E4BE2C26uLL, 0x6D4E3D514F59D312uLL, 0xDE6071EF4CFE8C7DuLL,
    0x15BB4F8BE788911CuLL, 0xA6950335E42FCE73uLL, 0xE13F79DC4FC83147uLL, 0x521135624C6F6E28uLL, 0x6E6B8C0F1807CF2FuLL, 0xDD45C0B11BA09040uLL, 0x9AEFBA58B0476F74uLL, 0x29C1F6E6B3E0301BuLL,
    0xC96C5795D7870F42uLL, 0x7A421B2BD420502DuLL, 0x3DE861C27FC7AF19uLL, 0x8EC62D7C7C60F076uLL, 0xB2BC941128085171uLL, 0x192D8AF2BAF0E1EuLL, 0x4638A2468048F12AuLL, 0xF516EEF883EFAE45uLL,
    0x3ECDD09C2899B324uLL, 0x8DE39C222B3EEC4BuLL, 0xCA49E6CB80D9137FuLL, 0x7967AA75837E4C10uLL, 0x451D1318D716ED17uLL, 0xF6335FA6D4B1B278uLL, 0xB199254F7F564D4CuLL, 0x2B769F17CF11223uLL,
    0xB4F7F6AD86B4690BuLL, 0x7D9BA1385133664uLL, 0x4073C0FA2EF4C950uLL, 0xF35D8C442D53963FuLL, 0xCF273529793B3738uLL, 0x7C0979977A9C6857uLL, 0x3BA3037ED17B9763uLL, 0x888D4FC0D2DCC80CuLL,
    0x435671A479AAD56DuLL, 0xF0783D1A7A0D8A02uLL, 0xB7D247F3D1EA7536uLL, 0x4FC0B4DD24D2A59uLL, 0x3886B22086258B5EuLL, 0x8BA8FE9E8582D431uLL, 0xCC0284772E652B05uLL, 0x7F2CC8C92DC2746AuLL,
    0x325B15E575E1C3D0uLL, 0x8175595B76469CBFuLL, 0xC6DF23B2DDA1638BuLL, 0x75F16F0CDE063CE4uLL, 0x498BD6618A6E9DE3uLL, 0xFAA59ADF89C9C28CuLL, 0xBD0FE036222E3DB8uLL, 0xE21AC88218962D7uLL,
    0xC5FA92EC8AFF7FB6uLL, 0x76D4DE52895820D9uLL, 0x317EA4BB22BFDFEDuLL, 0x8250E80521188082uLL, 0xBE2A516875702185uLL, 0xD041DD676D77EEAuLL, 0x4AAE673FDD3081DEuLL, 0xF9802B81DE97DEB1uLL,
    0x4FC0B4DD24D2A599uLL, 0xFCEEF8632775FAF6uLL, 0xBB44828A8C9205C2uLL, 0x86ACE348F355AADuLL, 0x34107759DB5DFBAAuLL, 0x873E3BE7D8FAA4C5uLL, 0xC094410E731D5BF1uLL, 0x73BA0DB070BA049EuLL,
    0xB86133D4DBCC19FFuLL, 0xB4F7F6AD86B4690uLL, 0x4CE50583738CB9A4uLL, 0xFFCB493D702BE6CBuLL, 0xC3B1F050244347CCuLL, 0x709FBCEE27E418A3uLL, 0x3735C6078C03E797uLL, 0x841B8AB98FA4B8F8uLL,
    0xADDA7C5F3C4488E3uLL, 0x1EF430E13FE3D78CuLL, 0x595E4A08940428B8uLL, 0xEA7006B697A377D7uLL, 0xD60ABFDBC3CBD6D0uLL, 0x6524F365C06C89BFuLL, 0x228E898C6B8B768BuLL, 0x91A0C532682C29E4uLL,
    0x5A7BFB56C35A3485uLL, 0xE955B7E8C0FD6BEAuLL, 0xAEFFCD016B1A94DEuLL, 0x1DD181BF68BDCBB1uLL, 0x21AB38D23CD56AB6uLL, 0x9285746C3F7235D9uLL, 0xD52F0E859495CAEDuLL, 0x6601423B97329582uLL,
    0xD041DD676D77EEAAuLL, 0x636F91D96ED0B1C5uLL, 0x24C5EB30C5374EF1uLL, 0x97EBA78EC690119EuLL, 0xAB911EE392F8B099uLL, 0x18BF525D915FEFF6uLL, 0x5F1528B43AB810C2uLL, 0xEC3B640A391F4FADuLL,
    0x27E05A6E926952CCuLL, 0x94CE16D091CE0DA3uLL, 0xD3646C393A29F297uLL, 0x604A2087398EADF8uLL, 0x5C3099EA6DE60CFFuLL, 0xEF1ED5546E415390uLL, 0xA8B4AFBDC5A6ACA4uLL, 0x1B9AE303C601F3CBuLL,
    0x56ED3E2F9E224471uLL, 0xE5C372919D851B1EuLL, 0xA26908783662E42AuLL, 0x114744C635C5BB45uLL, 0x2D3DFDAB61AD1A42uLL, 0x9E13B115620A452DuLL, 0xD9B9CBFCC9EDBA19uLL, 0x6A978742CA4AE576uLL,
    0xA14CB926613CF817uLL, 0x1262F598629BA778uLL, 0x55C88F71C97C584CuLL, 0xE6E6C3CFCADB0723uLL, 0xDA9C7AA29EB3A624uLL, 0x69B2361C9D14F94BuLL, 0x2E184CF536F3067FuLL, 0x9D36004B35545910uLL,
    0x2B769F17CF112238uLL, 0x9858D3A9CCB67D57uLL, 0xDFF2A94067518263uLL, 0x6CDCE5FE64F6DD0CuLL, 0x50A65C93309E7C0BuLL, 0xE388102D33392364uLL, 0xA4226AC498DEDC50uLL, 0x170C267A9B79833FuLL,
    0xDCD7181E300F9E5EuLL, 0x6FF954A033A8C131uLL, 0x28532E49984F3E05uLL, 0x9B7D62F79BE8616AuLL, 0xA707DB9ACF80C06DuLL, 0x14299724CC279F02uLL, 0x5383EDCD67C06036uLL, 0xE0ADA17364673F59uLL
};

# define CRC_STOP_SEC_CONST_64
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CRC_64_MODE == CRC_64_TABLE */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CRC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( (CRC_8_MODE == CRC_8_RUNTIME) || (CRC_8H2F_MODE == CRC_8H2F_RUNTIME) )
/**********************************************************************************************************************
 *  Crc_CalculateCRC8Runtime()
 *********************************************************************************************************************/
/*!
 * \brief      Performs runtime calculation of 8-bit CRC values
 * \details    Performs runtime calculation of 8-bit CRC values. This service is used by Crc_CalculateCRC8 and Crc_CalculateCRC8H2F routines.
 * \param[in]  Crc_Length
 * \param[in]  Crc_DataPtr
 * \param[in]  Crc_Value
 * \param[in]  Crc_Poly
 * \return     Calculated 8-bit CRC value
 * \pre        CRC8 runtime mode is configured
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
CRC_LOCAL FUNC(uint8, CRC_PRIVATE_CODE) Crc_CalculateCRC8Runtime (uint32 Crc_Length, Crc_DataRefType Crc_DataPtr, uint8 Crc_Value, uint8 Crc_Poly);
#endif

#if ( (CRC_32_MODE == CRC_32_RUNTIME) || (CRC_32P4_MODE == CRC_32P4_RUNTIME) )
/**********************************************************************************************************************
 *  Crc_CalculateCRC32Runtime()
 *********************************************************************************************************************/
/*!
 * \brief      Performs runtime calculation of 32-bit CRC values
 * \details    Performs runtime calculation of 32-bit CRC values. This service is used by Crc_CalculateCRC32 and Crc_CalculateCRC32P4 routines.
 * \param[in]  Crc_Length
 * \param[in]  Crc_DataPtr
 * \param[in]  Crc_Value
 * \param[in]  Crc_PolyReflected
 * \return     Calculated 32-bit CRC value
 * \pre        CRC32 runtime mode is configured
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
CRC_LOCAL FUNC(uint32, CRC_PRIVATE_CODE) Crc_CalculateCRC32Runtime (uint32 Crc_Length, Crc_DataRefType Crc_DataPtr, uint32 Crc_Value, uint32 Crc_PolyReflected);
#endif

#if (CRC_64_MODE == CRC_64_RUNTIME)
/**********************************************************************************************************************
 *  Crc_CalculateCRC64Runtime()
 *********************************************************************************************************************/
/*!
 * \brief      Performs runtime calculation of 64-bit CRC values
 * \details    Performs runtime calculation of 64-bit CRC values. This service is used by Crc_CalculateCRC64 routine.
 * \param[in]  Crc_Length
 * \param[in]  Crc_DataPtr
 * \param[in]  Crc_Value
 * \param[in]  Crc_PolyReflected
 * \return     Calculated 64-bit CRC value
 * \pre        CRC64 runtime mode is configured
 * \context    TASK
 * \reentrant  TRUE
 * \synchronous TRUE
 */
CRC_LOCAL FUNC(uint64, CRC_PRIVATE_CODE) Crc_CalculateCRC64Runtime (uint32 Crc_Length, Crc_DataRefType Crc_DataPtr, uint64 Crc_Value, uint64 Crc_PolyReflected);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if ( (CRC_8_MODE == CRC_8_RUNTIME) || (CRC_8H2F_MODE == CRC_8H2F_RUNTIME) )
/**********************************************************************************************************************
 *  Crc_CalculateCRC8Runtime()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CRC_LOCAL FUNC(uint8, CRC_PRIVATE_CODE) Crc_CalculateCRC8Runtime (uint32 Crc_Length, Crc_DataRefType Crc_DataPtr, uint8 Crc_Value, uint8 Crc_Poly)
{
  uint32 Crc_DataPtrAddr;
  uint8 Crc_LoopCounter;

  /* #10 Perform CRC calculation for each byte in Crc_DataPtr */
  for (Crc_DataPtrAddr = 0; Crc_DataPtrAddr < Crc_Length; Crc_DataPtrAddr++)
  {
    /* #20 XOR next byte of Crc_DataPtr with current CRC value. This is equivalent to calculating CRC value of concatenated bytes */
    Crc_Value ^= Crc_DataPtr[Crc_DataPtrAddr];

    /* #30 Perform modulo-2 division, a bit at a time */
    for (Crc_LoopCounter = 0; Crc_LoopCounter < 8; Crc_LoopCounter++)
    {
      /* #35 If MSB is 1, CRC value is XORed with polynomial */
      if ((Crc_Value & (uint8) 0x80u) > 0)
      {
        Crc_Value = ( (uint8)(Crc_Value << 1u) ) ^ Crc_Poly;
      }
      else
      {
        Crc_Value = (uint8) (Crc_Value << 1u);
      }
    }
  }

  return Crc_Value;
}
#endif

#if ( (CRC_32_MODE == CRC_32_RUNTIME) || (CRC_32P4_MODE == CRC_32P4_RUNTIME) )
/**********************************************************************************************************************
 *  Crc_CalculateCRC32Runtime()
 *********************************************************************************************************************/
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
CRC_LOCAL FUNC(uint32, CRC_PRIVATE_CODE) Crc_CalculateCRC32Runtime (uint32 Crc_Length, Crc_DataRefType Crc_DataPtr, uint32 Crc_Value, uint32 Crc_PolyReflected)
{
  uint32 Crc_DataPtrAddr;
  uint8 Crc_LoopCounter;

  /* #10 Perform CRC calculation for each byte in Crc_DataPtr */
  for (Crc_DataPtrAddr = 0; Crc_DataPtrAddr < Crc_Length; Crc_DataPtrAddr++)
  {

    /* #20 CRC32 routine considers reflection of input and output data. Instead of reflecting input and output data, a reflected polynomial is used */
    /* #21 XOR next byte of Crc_DataPtr with current CRC value. This is equivalent to calculating CRC value of concatenated bytes */
    Crc_Value ^= 0xFFuL & (uint32) Crc_DataPtr[Crc_DataPtrAddr];

    /* #30 Perform modulo-2 division, a bit at a time */
    for (Crc_LoopCounter = 0; Crc_LoopCounter < 8; Crc_LoopCounter++)
    {
      /* #35 If LSB is 1, CRC value is XORed with polynomial */
      /* #36 Direction of modulo-2 division is reverse compared to Crc8 and Crc16, due to the reflection of the polynomial */
      if ( (Crc_Value & 1u) > 0 )
      {
        Crc_Value = ( Crc_Value >> 1u ) ^ Crc_PolyReflected;
      }
      else
      {
        Crc_Value = Crc_Value >> 1u;
      }
    }
  }

  return Crc_Value;
}
#endif

#if (CRC_64_MODE == CRC_64_RUNTIME)
/**********************************************************************************************************************
 *  Crc_CalculateCRC64Runtime()
 *********************************************************************************************************************/
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
CRC_LOCAL FUNC(uint64, CRC_PRIVATE_CODE) Crc_CalculateCRC64Runtime (uint32 Crc_Length, Crc_DataRefType Crc_DataPtr, uint64 Crc_Value, uint64 Crc_PolyReflected)
{
  uint32 index;
  uint8 Crc_LoopCounter;

  /* #10 Perform CRC calculation over Crc_Length in Crc_DataPtr */
  for (index = 0; index < Crc_Length; index++)
  {

    /* #20 CRC64 routine considers reflection of input and output data. Instead of reflecting input and output data, a reflected polynomial is used */
    /* #21 XOR next byte of Crc_DataPtr with current CRC value. This is equivalent to calculating CRC value of concatenated bytes */
    Crc_Value ^= 0xFFuLL & (uint64) Crc_DataPtr[index];

    /* #30 Perform modulo-2 division, a bit at a time */
    for (Crc_LoopCounter = 0; Crc_LoopCounter < 8; Crc_LoopCounter++)
    {
      /* #35 If LSB is 1, CRC value is XORed with polynomial */
      /* #36 Direction of modulo-2 division is reverse compared to Crc8 and Crc16, due to the reflection of the polynomial */
      if ( (Crc_Value & 1uL) > 0 )
      {
        Crc_Value = ( Crc_Value >> 1uL ) ^ Crc_PolyReflected;
      }
      else
      {
        Crc_Value = Crc_Value >> 1uL;
      }
    }
  }

  return Crc_Value;
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Crc_CalculateCRC8
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(uint8, CRC_CODE) Crc_CalculateCRC8 (Crc_DataRefType Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8, boolean Crc_IsFirstCall)
{
  uint8 Crc_Value;

#if (CRC_8_MODE == CRC_8_TABLE)
  uint32 Crc_LoopCounter;
#endif

  /* #10 Set initial value depending on whether it's the first call or a subsequent call */
  if( Crc_IsFirstCall == TRUE )
  {
      Crc_Value = CRC_INITIAL_VALUE8;
  }
  else
  {
      Crc_Value = (CRC_FINAL_XOR_CRC8 ^ Crc_StartValue8);
  }

#if (CRC_8_MODE == CRC_8_TABLE)
  /*********** TABLE CALCULATION *******************************************************************************/
  /* #20 Calculate CRC value by using a table */
  for (Crc_LoopCounter = 0; Crc_LoopCounter < Crc_Length; Crc_LoopCounter++)
  {
    Crc_Value = Crc_Table8[Crc_Value ^ Crc_DataPtr[Crc_LoopCounter]];
  }
#endif

#if (CRC_8_MODE == CRC_8_RUNTIME)
  /*********** RUNTIME CALCULATION *****************************************************************************/
  /* #30 Calculate CRC value via CRC8 routine */
  Crc_Value = Crc_CalculateCRC8Runtime(Crc_Length, Crc_DataPtr, Crc_Value, CRC_POLYNOMIAL_8); /* SBSW_CRC_02 */
#endif

  /* #40 Final CRC value is XORed */
  return Crc_Value ^ CRC_FINAL_XOR_CRC8;
}

/***********************************************************************************************************************
 *  Crc_CalculateCRC8H2F
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(uint8, CRC_CODE) Crc_CalculateCRC8H2F (Crc_DataRefType Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8H2F, boolean Crc_IsFirstCall)
{
  uint8 Crc_Value;

#if (CRC_8H2F_MODE == CRC_8H2F_TABLE)
  uint32 Crc_LoopCounter;
#endif

  /* #10 Set initial value depending on whether it's the first call or a subsequent call */
  if (Crc_IsFirstCall == TRUE)
  {
    Crc_Value = CRC_INITIAL_VALUE8H2F;
  }
  else
  {
    Crc_Value = (CRC_FINAL_XOR_CRC8H2F ^ Crc_StartValue8H2F);
  }

#if (CRC_8H2F_MODE == CRC_8H2F_TABLE)
  /*********** TABLE CALCULATION *******************************************************************************/
  /* #20 Calculate CRC value by using a table */
  for (Crc_LoopCounter = 0; Crc_LoopCounter < Crc_Length; Crc_LoopCounter++)
  {
    Crc_Value = Crc_Table8H2F[Crc_Value ^ Crc_DataPtr[Crc_LoopCounter]];
  }
#endif

#if (CRC_8H2F_MODE == CRC_8H2F_RUNTIME)
  /*********** RUNTIME CALCULATION *****************************************************************************/
  /* #30 Calculate CRC value via CRC8 routine */
  Crc_Value = Crc_CalculateCRC8Runtime(Crc_Length, Crc_DataPtr, Crc_Value, CRC_POLYNOMIAL_8H2F); /* SBSW_CRC_02 */
#endif

  /* #40 Final CRC value is XORed */
  return Crc_Value ^ CRC_FINAL_XOR_CRC8H2F;
}

/***********************************************************************************************************************
 *  Crc_CalculateCRC16
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
FUNC(uint16, CRC_CODE) Crc_CalculateCRC16 (Crc_DataRefType Crc_DataPtr, uint32 Crc_Length, uint16 Crc_StartValue16, boolean Crc_IsFirstCall)
{
#if (CRC_16_MODE == CRC_16_TABLE)
  uint8 TableIndex;
#endif

  uint16 Crc_Value;
  uint32 Crc_DataPtrAddr;

  /* #10 Set initial value depending on whether it's the first call or a subsequent call */
  if (Crc_IsFirstCall == TRUE)
  {
    Crc_Value = CRC_INITIAL_VALUE16;
  }
  else
  {
    Crc_Value = Crc_StartValue16;
  }

#if (CRC_16_MODE == CRC_16_TABLE)
  /*********** TABLE CALCULATION *******************************************************************************/
  /* #20 Calculate CRC value by using a table */
  for (Crc_DataPtrAddr = 0; Crc_DataPtrAddr < Crc_Length; Crc_DataPtrAddr++)
  {
    TableIndex = ((uint8) (Crc_Value >> 8u)) ^ Crc_DataPtr[Crc_DataPtrAddr];
    Crc_Value = (Crc_Value << 8u) ^ Crc_Table16[TableIndex];
  }

#else
  /*********** RUNTIME CALCULATION *****************************************************************************/
  /* #30 Perform CRC calculation for each byte in Crc_DataPtr */
  for (Crc_DataPtrAddr = 0; Crc_DataPtrAddr < Crc_Length; Crc_DataPtrAddr++)
  {
    uint8 Crc_LoopCounter;

    /* #40 XOR next byte of Crc_DataPtr with current CRC value. This is equivalent to calculating CRC value of concatenated bytes */
    Crc_Value ^= ((uint16)Crc_DataPtr[Crc_DataPtrAddr] << 8u);

    /* #50 Perform modulo-2 division, a bit at a time */
    for (Crc_LoopCounter = 0; Crc_LoopCounter < 8; Crc_LoopCounter++)
    {
      /* #35 If MSB is 1, CRC value is XORed with polynomial */
      if ((Crc_Value & (uint16)0x8000u) > 0)
      {
        Crc_Value = ( (uint16)(Crc_Value << 1u) ) ^ CRC_POLYNOMIAL_16;
      }
      else
      {
        Crc_Value <<= 1u;
      }
    }
  }
#endif

  return Crc_Value;
}

/***********************************************************************************************************************
 *  Crc_CalculateCRC32
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(uint32, CRC_CODE) Crc_CalculateCRC32 (Crc_DataRefType Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32, boolean Crc_IsFirstCall)
{
  uint32 Crc_Value;

#if (CRC_32_MODE == CRC_32_TABLE)
  uint32 Crc_LoopCounter;
#endif

  /* #10 Set initial value depending on whether it's the first call or a subsequent call */
  if( Crc_IsFirstCall == TRUE )
  {
    Crc_Value = CRC_INITIAL_VALUE32;
  }
  else
  {
    Crc_Value = (CRC_FINAL_XOR_CRC32 ^ Crc_StartValue32);
  }

#if (CRC_32_MODE == CRC_32_TABLE)
  /*********** TABLE CALCULATION *******************************************************************************/
  /* #20 Calculate CRC value by using a table */
  for (Crc_LoopCounter = 0; Crc_LoopCounter < Crc_Length; Crc_LoopCounter++)
  {
    Crc_Value ^= 0xFFuL & (uint32) Crc_DataPtr[Crc_LoopCounter];
    Crc_Value = ( Crc_Value >> 8u ) ^ ( Crc_Table32 [(uint8)(Crc_Value )]);
  }
#else
  /*********** RUNTIME CALCULATION *****************************************************************************/
  /* #30 Calculate CRC value via CRC32 routine */
  Crc_Value = Crc_CalculateCRC32Runtime(Crc_Length, Crc_DataPtr, Crc_Value, CRC_POLYNOMIAL_32_REFLECT); /* SBSW_CRC_02 */
#endif

  /* #40 Final CRC value is XORed */
  return Crc_Value ^ CRC_FINAL_XOR_CRC32;
}

/***********************************************************************************************************************
 *  Crc_CalculateCRC32P4
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(uint32, CRC_CODE) Crc_CalculateCRC32P4 (Crc_DataRefType Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32, boolean Crc_IsFirstCall)
{
  uint32 Crc_Value;

#if (CRC_32P4_MODE == CRC_32P4_TABLE)
  uint32 Crc_LoopCounter;
#endif

  /* #10 Set initial value depending on whether it's the first call or a subsequent call */
  if( Crc_IsFirstCall == TRUE )
  {
    Crc_Value = CRC_INITIAL_VALUE32P4;
  }
  else
  {
    Crc_Value = (CRC_FINAL_XOR_CRC32P4 ^ Crc_StartValue32);
  }

#if (CRC_32P4_MODE == CRC_32P4_TABLE)
  /*********** TABLE CALCULATION *******************************************************************************/
  /* #20 Calculate CRC value by using a table */
  for (Crc_LoopCounter = 0; Crc_LoopCounter < Crc_Length; Crc_LoopCounter++)
  {
    Crc_Value ^= 0xFFuL & (uint32) Crc_DataPtr[Crc_LoopCounter];
    Crc_Value = ( Crc_Value >> 8u ) ^ ( Crc_Table32P4 [ (uint8)(Crc_Value )]);
  }
#else
  /*********** RUNTIME CALCULATION *****************************************************************************/
  /* #30 Calculate CRC value via CRC32 routine */
  Crc_Value = Crc_CalculateCRC32Runtime(Crc_Length, Crc_DataPtr, Crc_Value, CRC_POLYNOMIAL_32P4_REFLECT); /* SBSW_CRC_02 */
#endif

  /* #40 Final CRC value is XORed */
  return Crc_Value ^ CRC_FINAL_XOR_CRC32P4;
}

/***********************************************************************************************************************
 *  Crc_CalculateCRC64
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(uint64, CRC_CODE) Crc_CalculateCRC64 (Crc_DataRefType Crc_DataPtr, uint32 Crc_Length, uint64 Crc_StartValue64, boolean Crc_IsFirstCall)
{
  uint64 Crc_Value;

#if (CRC_64_MODE == CRC_64_TABLE)
  uint32 Crc_LoopCounter;
#endif

  /* #10 Set initial value depending on whether it's the first call or a subsequent call */
  if( Crc_IsFirstCall == TRUE )
  {
    Crc_Value = CRC_INITIAL_VALUE64;
  }
  else
  {
    Crc_Value = (CRC_FINAL_XOR_CRC64 ^ Crc_StartValue64);
  }

#if (CRC_64_MODE == CRC_64_TABLE)
  /*********** TABLE CALCULATION *******************************************************************************/
  /* #20 Calculate CRC value by using a table */
  for (Crc_LoopCounter = 0; Crc_LoopCounter < Crc_Length; Crc_LoopCounter++)
  {
    Crc_Value ^= 0xFFuL & (uint64) Crc_DataPtr[Crc_LoopCounter];
    Crc_Value = ( Crc_Value >> 8uLL ) ^ ( Crc_Table64 [(uint8)(Crc_Value)]);
  }
#else
  /*********** RUNTIME CALCULATION *****************************************************************************/
  /* #30 Calculate CRC value via CRC64 routine */
  Crc_Value = Crc_CalculateCRC64Runtime(Crc_Length, Crc_DataPtr, Crc_Value, CRC_POLYNOMIAL_64_REFLECT); /* SBSW_CRC_02 */
#endif

  /* #40 Final CRC value is XORed */
  return Crc_Value ^ CRC_FINAL_XOR_CRC64;
}

#if (CRC_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  Crc_GetVersionInfo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRC_CODE) Crc_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, CRC_APPL_DATA) Versioninfo)
{
  if (Versioninfo != NULL_PTR)
  {
    Versioninfo->vendorID = CRC_VENDOR_ID; /* SBSW_CRC_01 */
    Versioninfo->moduleID = CRC_MODULE_ID; /* SBSW_CRC_01 */
    Versioninfo->sw_major_version = CRC_SW_MAJOR_VERSION; /* SBSW_CRC_01 */
    Versioninfo->sw_minor_version = CRC_SW_MINOR_VERSION; /* SBSW_CRC_01 */
    Versioninfo->sw_patch_version = CRC_SW_PATCH_VERSION; /* SBSW_CRC_01 */
  }
}
#endif

#define CRC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Module specific MISRA deviations:

MD_CRC_8.7_FileScopeStatic:
    Reason: CRC Calculation tables are defined outside of functions, in which they are used.
    This is done for reason of structural clarity.
    Risk: No risk.
    Prevention: Tables could be defined within calculation routines.

*/

/* START_COVERAGE_JUSTIFICATION
    \ID COV_CRC_COMPATIBILITY
      \ACCEPT XF
      \ACCEPT TX
      \REASON [COV_MSR_COMPATIBILITY]
END_COVERAGE_JUSTIFICATION */

/* SBSW_JUSTIFICATION_BEGIN
   \ID SBSW_CRC_01
    \DESCRIPTION The function Crc_GetVersionInfo writes to the object referenced by parameter VersionInfoPtr
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameter VersionInfoPtr is valid.

   \ID SBSW_CRC_02
    \DESCRIPTION Local function is called with pointer parameter Crc_DataPtr which was passed from caller of CRC library.
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameter Crc_DataPtr is valid.
SBSW_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Crc.c
 *********************************************************************************************************************/
