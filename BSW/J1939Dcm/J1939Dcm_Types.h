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
 *         File:  J1939Dcm_Types.h
 *    Component:  MICROSAR Diagnostic Communication Manager for SAE J1939
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  global type definitions and macros for J1939 DCM
 *
 *********************************************************************************************************************/

#if !defined (J1939DCM_TYPES_H)
#define J1939DCM_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* --- SAE definitions: parameter group numbers of the diagnostic messages ------------------------------------------*/
#define J1939DCM_DM1_PGN           (uint32)0x0000FECAUL
#define J1939DCM_DM2_PGN           (uint32)0x0000FECBUL
#define J1939DCM_DM3_PGN           (uint32)0x0000FECCUL
#define J1939DCM_DM4_PGN           (uint32)0x0000FECDUL
#define J1939DCM_DM5_PGN           (uint32)0x0000FECEUL /* OBD only */
#define J1939DCM_DM6_PGN           (uint32)0x0000FECFUL /* OBD only */
#define J1939DCM_DM7_PGN           (uint32)0x0000E300UL /* currently not supported */
#define J1939DCM_DM8_PGN           (uint32)0x0000FED0UL /* currently not supported */
#define J1939DCM_DM9_PGN           (uint32)0xFFFFFFFFUL /* not specified */
#define J1939DCM_DM10_PGN          (uint32)0x0000FED2UL /* currently not supported */
#define J1939DCM_DM11_PGN          (uint32)0x0000FED3UL
#define J1939DCM_DM12_PGN          (uint32)0x0000FED4UL /* OBD only */
#define J1939DCM_DM13_PGN          (uint32)0x0000DF00UL
#define J1939DCM_DM14_PGN          (uint32)0x0000D900UL /* needed for OEM */
#define J1939DCM_DM15_PGN          (uint32)0x0000D800UL /* needed for OEM */
#define J1939DCM_DM16_PGN          (uint32)0x0000D700UL /* needed for OEM */
#define J1939DCM_DM17_PGN          (uint32)0x0000D600UL /* needed for OEM */
#define J1939DCM_DM18_PGN          (uint32)0x0000D400UL /* needed for OEM */
#define J1939DCM_DM19_PGN          (uint32)0x0000D300UL /* OBD only */
#define J1939DCM_DM20_PGN          (uint32)0x0000C200UL /* OBD only */
#define J1939DCM_DM21_PGN          (uint32)0x0000C100UL /* OBD only */
#define J1939DCM_DM22_PGN          (uint32)0x0000C300UL /* currently not supported */
#define J1939DCM_DM23_PGN          (uint32)0x0000FDB5UL /* OBD only */
#define J1939DCM_DM24_PGN          (uint32)0x0000FDB6UL
#define J1939DCM_DM25_PGN          (uint32)0x0000FDB7UL
#define J1939DCM_DM26_PGN          (uint32)0x0000FDB8UL /* OBD only */
#define J1939DCM_DM27_PGN          (uint32)0x0000FD82UL
#define J1939DCM_DM28_PGN          (uint32)0x0000FD80UL /* OBD only */
#define J1939DCM_DM29_PGN          (uint32)0x00009E00UL /* OBD only */
#define J1939DCM_DM30_PGN          (uint32)0x0000A400UL /* currently not supported */
#define J1939DCM_DM31_PGN          (uint32)0x0000A300UL
#define J1939DCM_DM32_PGN          (uint32)0x0000A200UL /* currently not supported */
#define J1939DCM_DM33_PGN          (uint32)0x0000A100UL /* currently not supported */
#define J1939DCM_DM34_PGN          (uint32)0x0000A000UL /* currently not supported */
#define J1939DCM_DM35_PGN          (uint32)0x00009F00UL
#define J1939DCM_DM53_PGN          (uint32)0x0000FCD1UL
#define J1939DCM_DM54_PGN          (uint32)0x0000FCD2UL
#define J1939DCM_DM55_PGN          (uint32)0x0000FCD3UL


/* --- SAE definitions: default priorities --------------------------------------------------------------------------*/
#define J1939DCM_DM1_PRIORITY      6
#define J1939DCM_DM2_PRIORITY      6
#define J1939DCM_DM3_PRIORITY      6
#define J1939DCM_DM4_PRIORITY      6
#define J1939DCM_DM5_PRIORITY      6
#define J1939DCM_DM6_PRIORITY      6
#define J1939DCM_DM7_PRIORITY      6
#define J1939DCM_DM8_PRIORITY      6
#define J1939DCM_DM9_PRIORITY      6
#define J1939DCM_DM10_PRIORITY     6
#define J1939DCM_DM11_PRIORITY     6
#define J1939DCM_DM12_PRIORITY     6
#define J1939DCM_DM13_PRIORITY     6
#define J1939DCM_DM14_PRIORITY     6
#define J1939DCM_DM15_PRIORITY     6
#define J1939DCM_DM16_PRIORITY     6
#define J1939DCM_DM17_PRIORITY     6
#define J1939DCM_DM18_PRIORITY     6
#define J1939DCM_DM19_PRIORITY     7
#define J1939DCM_DM20_PRIORITY     6
#define J1939DCM_DM21_PRIORITY     6
#define J1939DCM_DM22_PRIORITY     6
#define J1939DCM_DM23_PRIORITY     6
#define J1939DCM_DM24_PRIORITY     6
#define J1939DCM_DM25_PRIORITY     6
#define J1939DCM_DM26_PRIORITY     6
#define J1939DCM_DM27_PRIORITY     6
#define J1939DCM_DM28_PRIORITY     6
#define J1939DCM_DM29_PRIORITY     6
#define J1939DCM_DM30_PRIORITY     6
#define J1939DCM_DM31_PRIORITY     6
#define J1939DCM_DM32_PRIORITY     6
#define J1939DCM_DM33_PRIORITY     6
#define J1939DCM_DM34_PRIORITY     6
#define J1939DCM_DM35_PRIORITY     6
#define J1939DCM_PRIORITY_DEFAULT  6


/* --- DM message identifier ----------------------------------------------------------------------------------------*/
#define J1939DCM_DM_UNDEF          0
#define J1939DCM_DM1               1
#define J1939DCM_DM2               2
#define J1939DCM_DM3               3
#define J1939DCM_DM4               4
#define J1939DCM_DM5               5
#define J1939DCM_DM6               6
#define J1939DCM_DM7               7
#define J1939DCM_DM8               8
#define J1939DCM_DM9               9
#define J1939DCM_DM10              10
#define J1939DCM_DM11              11
#define J1939DCM_DM12              12
#define J1939DCM_DM13              13
#define J1939DCM_DM14              14
#define J1939DCM_DM15              15
#define J1939DCM_DM16              16
#define J1939DCM_DM17              17
#define J1939DCM_DM18              18
#define J1939DCM_DM19              19
#define J1939DCM_DM20              20
#define J1939DCM_DM21              21
#define J1939DCM_DM22              22
#define J1939DCM_DM23              23
#define J1939DCM_DM24              24
#define J1939DCM_DM25              25
#define J1939DCM_DM26              26
#define J1939DCM_DM27              27
#define J1939DCM_DM28              28
#define J1939DCM_DM29              29
#define J1939DCM_DM30              30
#define J1939DCM_DM31              31
#define J1939DCM_DM32              32
#define J1939DCM_DM33              33
#define J1939DCM_DM34              34
#define J1939DCM_DM35              35
#define J1939DCM_DM36              36
#define J1939DCM_DM37              37
#define J1939DCM_DM38              38
#define J1939DCM_DM39              39
#define J1939DCM_DM40              40
#define J1939DCM_DM41              41
#define J1939DCM_DM42              42
#define J1939DCM_DM43              43
#define J1939DCM_DM44              44
#define J1939DCM_DM45              45
#define J1939DCM_DM46              46
#define J1939DCM_DM47              47
#define J1939DCM_DM48              48
#define J1939DCM_DM49              49
#define J1939DCM_DM50              50
#define J1939DCM_DM51              51
#define J1939DCM_DM52              52
#define J1939DCM_DM53              53
#define J1939DCM_DM54              54
#define J1939DCM_DM55              55
#define J1939DCM_DM_MAX            56


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* --- data types ---------------------------------------------------------------------------------------------------*/
typedef uint8  J1939Dcm_NodeHdlType;
typedef uint8  J1939Dcm_ChannelHdlType;
typedef uint8  J1939Dcm_MessageHdlType;
typedef uint16 J1939Dcm_ChannelSupportType;      /* each bit represents one channel (Bit0 = ChannelHdl 0...)         */

/* --- enumerations -------------------------------------------------------------------------------------------------*/
typedef uint8 J1939Dcm_StateType;
#define J1939DCM_STATE_ONLINE      (J1939Dcm_StateType)0        /* SWS_J1939Dcm_00123 */
#define J1939DCM_STATE_OFFLINE     (J1939Dcm_StateType)1        /* SWS_J1939Dcm_00123 */

typedef uint8 J1939DcmBusType;
/* note: order of enumeration derived from SAE definition for DM13 */
#define J1939DCM_CURRENT_NETWORK   (J1939DcmBusType)0
#define J1939DCM_J1587             (J1939DcmBusType)1
#define J1939DCM_J1922             (J1939DcmBusType)2
#define J1939DCM_J1939_NETWORK_1   (J1939DcmBusType)3
#define J1939DCM_J1939_NETWORK_2   (J1939DcmBusType)4
#define J1939DCM_ISO9141           (J1939DcmBusType)5
#define J1939DCM_J1850             (J1939DcmBusType)6
#define J1939DCM_OTHER             (J1939DcmBusType)7
#define J1939DCM_J1939_NETWORK_3   (J1939DcmBusType)8

typedef uint8 J1939Dcm_TxPathType;
#define J1939DCM_TX_PATH_TP        (J1939Dcm_TxPathType)0
#define J1939DCM_TX_PATH_IF        (J1939Dcm_TxPathType)1

typedef uint8  J1939Dcm_MemStatusType;
#define J1939DCM_MEM_STATUS_PROCEED      0
#define J1939DCM_MEM_STATUS_BUSY         1
#define J1939DCM_MEM_STATUS_OPCOMPLETE   4
#define J1939DCM_MEM_STATUS_OPFAILED     5

typedef uint8  J1939Dcm_MemCommandType;
#define J1939DCM_MEM_CMD_ERASE           0
#define J1939DCM_MEM_CMD_READ            1
#define J1939DCM_MEM_CMD_WRITE           2
#define J1939DCM_MEM_CMD_STATUS          3
#define J1939DCM_MEM_CMD_OPCOMPLETE      4
#define J1939DCM_MEM_CMD_OPFAILED        5
#define J1939DCM_MEM_CMD_BOOTLOAD        6
#define J1939DCM_MEM_CMD_EDCPGEN         7

typedef uint8  J1939Dcm_MemDataType;
#define J1939DCM_MEM_DATA_BINARY         J1939DCM_DM16
#define J1939DCM_MEM_DATA_BOOTLOAD       J1939DCM_DM17
#define J1939DCM_MEM_DATA_SECURITY       J1939DCM_DM18
#define J1939DCM_MEM_DATA_INVALID        J1939DCM_DM_UNDEF

typedef uint8  J1939Dcm_MemPointerType;
#define J1939DCM_MEM_PTRTYPE_DIRECT      0
#define J1939DCM_MEM_PTRTYPE_SPATIAL     1

typedef uint32 J1939Dcm_MemErrorType;
#define J1939DCM_MEM_ERROR_NONE          0x00000000UL   /* No Error                                                                                                 */
#define J1939DCM_MEM_ERROR_UNDEF         0x00000001UL   /* Error NOT identified                                                                                     */
#define J1939DCM_MEM_ERROR_BUSY_OTHER    0x00000002UL   /* Currently processing for someone else                                                                    */
#define J1939DCM_MEM_ERROR_BUSY_ERASE    0x00000010UL   /* Currently processing Erase Request                                                                       */
#define J1939DCM_MEM_ERROR_BUSY_READ     0x00000011UL   /* Currently processing Read Request                                                                        */
#define J1939DCM_MEM_ERROR_BUSY_WRITE    0x00000012UL   /* Currently processing Write Request                                                                       */
#define J1939DCM_MEM_ERROR_BUSY_STATUS   0x00000013UL   /* Currently processing Status Request                                                                      */
#define J1939DCM_MEM_ERROR_BUSY_BOOT     0x00000016UL   /* Currently processing Boot Load Request                                                                   */
#define J1939DCM_MEM_ERROR_BUSY_EDCP     0x00000017UL   /* Currently processing EDCP Generation Request                                                             */
#define J1939DCM_MEM_ERROR_BUSY_UNDEF    0x0000001FUL   /* Currently processing unspecified request from this address                                               */
#define J1939DCM_MEM_ERROR_EDC_PARAM     0x00000020UL   /* EDC parameter not correct for data stream                                                                */
#define J1939DCM_MEM_ERROR_WRITE_RAM     0x00000021UL   /* RAM did not verify on Write                                                                              */
#define J1939DCM_MEM_ERROR_WRITE_FLASH   0x00000022UL   /* FLASH did not verify on Write                                                                            */
#define J1939DCM_MEM_ERROR_WRITE_PROM    0x00000023UL   /* PROM did not verify on Write                                                                             */
#define J1939DCM_MEM_ERROR_INTERNAL      0x00000024UL   /* Internal failure preventing request (i.e. within the ECU)                                                */
#define J1939DCM_MEM_ERROR_ADDR_GENERAL  0x00000100UL   /* Addressing or DATA General Error                                                                         */
#define J1939DCM_MEM_ERROR_ADDR_BOUND    0x00000101UL   /* Addressing Error - Address not on a valid boundary (Block, Word, Object, etc.)                           */
#define J1939DCM_MEM_ERROR_ADDR_LENGTH   0x00000102UL   /* Addressing Error - Length not valid for memory structure and operation                                   */
#define J1939DCM_MEM_ERROR_ADDR_EXCEED   0x00000103UL   /* Addressing Error - required memory exceeded available memory                                             */
#define J1939DCM_MEM_ERROR_ADDR_DATA     0x00000104UL   /* Addressing Error - requested operation requires prior erase of DATA memory                               */
#define J1939DCM_MEM_ERROR_ADDR_PROG     0x00000105UL   /* Addressing Error - requested operation requires prior erase of PROGRAM memory                            */
#define J1939DCM_MEM_ERROR_ADDR_PROG2    0x00000106UL   /* Addressing Error - requested operation requires prior execution transfer and erase of PROGRAM memory     */
#define J1939DCM_MEM_ERROR_ADDR_BOOT_EXE 0x00000107UL   /* Addressing Error - requested address for Boot Loader execution transfer is NOT within executable memory  */
#define J1939DCM_MEM_ERROR_ADDR_BOOT_BND 0x00000108UL   /* Addressing Error - requested address for Boot Loader execution transfer is NOT on valid boundary         */
#define J1939DCM_MEM_ERROR_DATA_RANGE    0x00000109UL   /* DATA Error - data does NOT conform to expected or allowed value ranges                                   */
#define J1939DCM_MEM_ERROR_DATA_VALUE    0x0000010AUL   /* DATA Error - NAME does NOT conform to expected value                                                     */
#define J1939DCM_MEM_ERROR_SEC_GENRAL    0x00001000UL   /* Security Error General                                                                                   */
#define J1939DCM_MEM_ERROR_SEC_PASSWD    0x00001001UL   /* Security Error - Invalid Password                                                                        */
#define J1939DCM_MEM_ERROR_SEC_USER      0x00001002UL   /* Security Error - Invalid User Level                                                                      */
#define J1939DCM_MEM_ERROR_SEC_KEY       0x00001003UL   /* Security Error - Invalid Key {Seed}                                                                      */
#define J1939DCM_MEM_ERROR_SEC_NODIAG    0x00001004UL   /* Security Error - NOT in Diagnostic mode                                                                  */
#define J1939DCM_MEM_ERROR_SEC_NODEV     0x00001005UL   /* Security Error - NOT in Engineering or Development mode                                                  */
#define J1939DCM_MEM_ERROR_SEC_ENGINE    0x00001006UL   /* Security Error - Engine running                                                                          */
#define J1939DCM_MEM_ERROR_SEC_NOPARK    0x00001007UL   /* Security Error - Vehicle NOT in 'Park' or otherwise NOT stationary                                       */
#define J1939DCM_MEM_ERROR_ABORT_EXTERN  0x00010000UL   /* Abort from external to normal software process                                                           */
#define J1939DCM_MEM_ERROR_RETRIES       0x00010001UL   /* Too Many Retries - module exceeding a set number of retries                                              */
#define J1939DCM_MEM_ERROR_TIMEOUT_RESP  0x00010002UL   /* NO response in the time allowed                                                                          */
#define J1939DCM_MEM_ERROR_TIMEOUT_START 0x00010003UL   /* Transport of data NOT initiated within the time allowed                                                  */
#define J1939DCM_MEM_ERROR_TIMEOUT_CMPLT 0x00010004UL   /* Transport of data NOT completed within the time allowed                                                  */
#define J1939DCM_MEM_ERROR_UNAVAILABLE   0x00FFFFFFUL   /* No Error Indicator Available                                                                             */



/* --- function pointers and structs for memory access --------------------------------------------------------------*/
/* parameters of DM14: Memory Access Request */
typedef struct
{
  uint32                  Pointer;
  uint16                  Length;
  uint16                  Key;
  uint8                   PointerExtension;
  J1939Dcm_MemPointerType PointerType;
  J1939Dcm_MemCommandType Command;
} J1939Dcm_MemReqParamType;

/* parameters of DM15: Memory Access Response */
typedef struct
{
  J1939Dcm_MemErrorType   ErrorIndicator;
  uint16                  Length;
  uint16                  Seed;
  J1939Dcm_MemStatusType  Status;
  uint8                   EDCPExtension;
} J1939Dcm_MemRespParamType;


/* addressing info for received TP messages */
typedef struct
{
  NetworkHandleType ChannelId;
  uint8 SourceAddress;
  uint8 DestinationAddress;
} J1939Dcm_AddressInfoType;


#endif  /* J1939DCM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Dcm_Types.h
 *********************************************************************************************************************/
