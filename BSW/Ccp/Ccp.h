#if defined ( C_KOMMENTAR_VECTOR )
/*****************************************************************************
|
|                 SINGLE SOURCE for internal usage only
|
|    *****            D O   N O T   D E L I V E R         *****
|
| Instructions for filtering with ORGANI:
|  - remove all parts of the code that are encapsulated with "C_COMMENT_VECTOR"
|
|****************************************************************************/
#endif

/*****************************************************************************
| Project Name:   C C P - Driver
|    File Name:   Ccp.h
|
|  Description: 
|   CCP driver declarations.
|   CANape CAN Calibration Tool.
|
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2001-2017 by Vector Informatik GmbH.      All rights reserved.
|
|       This software is copyright protected and 
|       proprietary to Vector Informatik GmbH.
|       Vector Informatik GmbH grants to you only
|       those rights as set out in the license conditions.
|       All other rights remain with Vector Informatik GmbH.
| 
|       Diese Software ist urheberrechtlich geschuetzt. 
|       Vector Informatik GmbH raeumt Ihnen an dieser Software nur 
|       die in den Lizenzbedingungen ausdruecklich genannten Rechte ein.
|       Alle anderen Rechte verbleiben bei Vector Informatik GmbH.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| Bus          Sabine Buecherl           Vector Informatik GmbH
| Ds           Sven Deckardt             Vector Informatik GmbH
| Eta          Edgar Tongoona            Vector Informatik GmbH
| Hp           Armin Happel              Vector Informatik GmbH
| Hr           Andreas Herkommer         Vector Informatik GmbH
| Tri          Frank Triem               Vector Informatik GmbH
| Za           Rainer Zaiser             Vector Informatik GmbH
| Bwr          Brandon Root              Vector CANtech Inc.
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
|  Date       Version  Author  Description
| ----------  -------  ------  -----------------------------------------------
| 2000-24-09  1.29.00  Za      - New define CCP_CHECKSUM_BLOCKSIZE
| 2000-29-11  1.30.00  Za      - #ifndef CCP_EXTERNAL_STATION_ID
| 2001-08-02  1.31.00  Za      - new define CCP_DAQ_BASE_ADDR
|                              - new function Ccp_GetDaqPointer
| 2001-30-05  1.32.00  Za      - Reserved word "data" in KEIL Compiler for C5x5
|                              - Prefix CCP_ for all #defines
| 2001-14-09  1.33.00  Za      - #define CCP_ODT_ENTRY_SIZE
|                              - #define CCP_INTEL,CCP_MOTOROLA
| 2001-28-10  1.34.00  Za      - Ccp_Send return value removed
|                              - Transmission error handling should be done by the user
| 2002-08-04  1.35.00  Za      - #define CCP_CPUTYPE_32BIT
|                              - Max checksum block size is DWORD on 32 bit CPUs
| 2002-02-06  1.36.00  Za      - #undef CCP_DAQ for drivers without DAQ fixed
|                              - double - float conversion for SHORT_UPLOAD, DNLOAD and DAQ
| 2002-17-07  1.37.00  Ds      - Fixed the version nr. because the version was in 
|                                the comment 1.36 but 135 was define.
|                              - Set #define CCP_DRIVER_VERSION to 137
| 2002-14-11  1.37.01  Hp      - define CCP_MAX_DAQ only if CCP_DAQ is defined
| 2002-27-11  1.37.02  Ds      - delete the query of extended id
| 2003-05-28  1.37.02  Bus     - added V_MEMROM0
| 2003-08-11  1.37.03  Tri     - implemented P_MEM_ROM and P_MEM_RAM to support M16C Mitsubishi.
| 2003-10-14  1.38.00  Tri     - version skipped due to special version for TMS320
| 2003-10-14  1.39.00  Tri     - version skipped due to special version for TMS320
| 2003-10-14  1.40.00  Tri     - merge of versions: 1.37.03, 1.37.02
| 2003-10-16  1.41.00  Ds      - minor bugfix set ROM to CCP_ROM  
| 2003-10-16  1.41.01  Ds      - change the position of CCP_ROM  
| 2003-10-21  1.42.00  Tri     - change the position of CCP_ROM
| 2003-11-14  1.42.01  Tri     - implemented type cast due to compiler warnings
| 2003-11-20  1.42.02  Tri     ESCAN00007060: Temporary deconnection not possible
| 2004-01-30  1.42.03  Tri     ESCAN00007384: Compiler warning due implicit type cast
| 2004-03-04  1.43.00  Tri     ESCAN00007799: Include structure adaption for GENy
| 2004-11-17  1.44.00  Tri     ESCAN00009242: Add comment where an emulator can stop due to an unknown TRAP
|                              ESCAN00010225: Measuring data from ROM does not work correctly
|                              ESCAN00010255: Missing cast
| 2005-05-23  1.45.00  Tri     ESCAN00012355: Memory access by application
| 2005-06-30  1.46.00  Tri     ESCAN00012616: Comment for LizDb parser added
|                      Tri     ESCAN00012700: place ccpWrite in far page for Star12
| 2005-11-21  1.47.00  Tri     ESCAN00014171: Compiler error in checksum calculation
| 2006-06-12  1.48.00  Tri     ESCAN00016079: WORD and DWORD access to BYTE aligned memory
|                              ESCAN00016157: Demo disclaimer added
|                              ESCAN00014625: Warning because of missing return value
| 2006-08-31  1.49.00  Tri     ESCAN00017503: Replace P_MEM_ROM by MEMORY_FAR resp. V_MEMRAM2_FAR
| 2006-11-06  1.50.00  Tri     ESCAN00018251: Support of small memory model
| 2007-03-07  1.51.00  Eta     ESCAN00019311: Remove initialized RAM vars: ccpResourceMask and ccpLastSeed
| 2009-03-12  1.51.01  Hr      ESCAN00031915:
|                              ESCAN00033120: Narrowing or signed-to-unsigned type conversion
| 2009-03-12  1.52.00  Bwr     ESCAN00033270: Compiler error when CCP_ODT_ENTRY_SIZE is enabled
|                              ESCAN00033831: Corrected handling of CCP_ODT_ENTRY_SIZE
| 2009-09-07  1.52.01  Hr      ESCAN00037015: Compile error occurs when DAQ is enabled
| 2009-02-22  1.52.02  Hr      ESCAN00040819: CCP_ODT_ENTRY_SIZE: compile errors occur
| 2012-01-16  2.00.00  Hr      ESCAN00056051: Create ASR compatible CCP
| 2012-03-22  2.00.01  Hr      ESCAN00057792: Compile Error: Ccp_GetVersionInfo writes intanceID which is not present
| 2012-03-26  2.00.02  Hr      ESCAN00057864: Warning from delivery package manager
| 2012-04-10  2.00.03  Hr      ESCAN00057864: CCP_DISCONNECT does not verify the Node ID
| 2012-06-01                   ESCAN00059218: Send Queue not working correctly
| 2012-07-16  2.00.04  Hr      ESCAN00058361: GET_DAQ_SIZE does not verify CAN-ID
| 2012-08-22  2.00.05  Hr      ESCAN00060907: Incorrect component release version
| 2012-10-01  2.00.06  Hr      ESCAN00061397: GET_DAQ_SIZE does not update the "NUM_ODT" & "First PID" when receiving an invalid ResponseID
| 2013-03-04  2.00.07  Hr      ESCAN00062363: Prototypes of local functions not encapsulated use case specific
| 2013-05-08  2.00.08  Hr      ESCAN00067222: Compiler warning: large type was implicitly cast to small type
| 2014-07-18                   ESCAN00076196: Broadcast address feature not configurable
| 2014-12-09  2.01.00  Hr      ESCAN00078512: AR4-755: Support IdentityManager Config
| 2015-02-27  2.01.01  Hr      ESCAN00066589: Command plausibility check incomplete
| 2017-01-10  3.00.00  Hr      ESCAN00093486: Rework of component for AR4/Cfg5 based on Cp_AsrCcp
|                      Hr      ESCAN00093487: Compiler warning: warning #82-D
|                      Hr      ESCAN00093488: Frame end check during DAQ assembly not correct
|***************************************************************************/


#ifndef __CCP_H_
#define __CCP_H_

#include "ComStack_Types.h"
#include "Ccp_Cfg.h"
#include "Ccp_PBcfg.h"
#include "SchM_Ccp.h"

#ifndef C_DISABLE_CCP

/* Declare CCP-protocol version */
#define CCP_VERSION_MAJOR   0x02
#define CCP_VERSION_MINOR   0x01

#define CCP_SW_MAJOR_VERSION 0x03
#define CCP_SW_MINOR_VERSION 0x00
#define CCP_SW_PATCH_VERSION 0x00

/* Declare module implementation version number */
/* ##V_CFG_MANAGEMENT ##CQProject : Cp_Asr4Ccp CQComponent : Implementation */
#define CP_ASR4CCP_VERSION         0x0300
#define CP_ASR4CCP_RELEASE_VERSION 0x00

/* Module versions for downward compatibility. */
#define CCP_DRIVER_VERSION 100
#define CCP_DRIVER_RELEASE_VERSION 0

#define CCP_VENDOR_ID 30u
#define CCP_MODULE_ID 255u

/***************************************************************************/
/* Default macros                                                          */
/***************************************************************************/

/* Interrupt enable and disable functions */
#if defined ( CCP_DISABLE_INTERRUPT )
#else
#  define CCP_DISABLE_INTERRUPT()         SchM_Enter_Ccp_CCP_EXCLUSIVE_AREA_0()
#endif
#if defined ( CCP_ENABLE_INTERRUPT )
#else
#  define CCP_ENABLE_INTERRUPT()          SchM_Exit_Ccp_CCP_EXCLUSIVE_AREA_0()
#endif

/* Ccp activation control */
# define kCcp_Control_Enable  0u
# define kCcp_Control_Disable 1u

/* Initialization status of CCP. */
#define CCP_UNINIT   0
#define CCP_INIT     1

/***************************************************************************/
/* Type definitions                                                        */
/***************************************************************************/
/* Development errors */
/* Definition of the API ID */
#define CCP_INIT_SERVICE_ID                   0u
#define CCP_DAQ_SERVICE_ID                    1u
#define CCP_MAINFUNCTION_SERVICE_ID           2u
#define CCP_COMMAND_SERVICE_ID                3u
#define CCP_SEND_CALLBACK_SERVICE_ID          4u
#define CCP_SENDCRM_SERVICE_ID                5u
#define CCP_QUEUEINIT_SERVICE_ID              6u
#define CCP_QUEUEWRITE_SERVICE_ID             7u
#define CCP_TXCONFIRMATION_SERVICE_ID         8u
#define CCP_RXINDICATION_SERVICE_ID           9u
#define CCP_SEND_SERVICE_ID                  10u
#define CCP_GETVERSIONINFO_SERVICE_ID        11u
#define CCP_CANGETCURRENTCANID_SERVICE_ID    12u

/* Definition of errors/exceptions */
#define CCP_E_NOT_INITIALIZED                 2u
#define CCP_E_INVALID_PDUID                   3u
#define CCP_E_NULL_POINTER                    4u

/* Basic */
#define CC_CONNECT           0x01
#define CC_SET_MTA           0x02
#define CC_DNLOAD            0x03
#define CC_UPLOAD            0x04
#define CC_TEST              0x05 /* V2.1 */
#define CC_START_STOP        0x06
#define CC_DISCONNECT        0x07
#define CC_START_STOP_ALL    0x08 /* V2.1 */
#define CC_SHORT_UPLOAD      0x0F
#define CC_GET_DAQ_SIZE      0x14
#define CC_SET_DAQ_PTR       0x15
#define CC_WRITE_DAQ         0x16
#define CC_EXCHANGE_ID       0x17
#define CC_GET_CCP_VERSION   0x1B /* V2.1 */
#define CC_DNLOAD6           0x23

/* Optional */
#define CC_GET_CAL_PAGE      0x09
#define CC_SET_S_STATUS      0x0C
#define CC_GET_S_STATUS      0x0D
#define CC_BUILD_CHKSUM      0x0E
#define CC_CLEAR_MEMORY      0x10
#define CC_SET_CAL_PAGE      0x11
#define CC_GET_SEED          0x12
#define CC_UNLOCK            0x13
#define CC_PROGRAM           0x18
#define CC_MOVE_MEMORY       0x19
#define CC_DIAG_SERVICE      0x20
#define CC_ACTION_SERVICE    0x21
#define CC_PROGRAM6          0x22 

/* Vector extensions */
#define CC_PROGRAM_PREPARE   0x1E   /* Prepare for flash kernel download */
#define CC_PROGRAM_START     0x1F   /* Start flash kernel at MTA[0] */

/* Returncodes */
#define CRC_OK                 0x00

/* C1 */
#define CRC_CMD_BUSY           0x10
#define CRC_DAQ_BUSY           0x11
#define CRC_KEY_REQUEST        0x18
#define CRC_STATUS_REQUEST     0x19

/* C2 */
#define CRC_COLD_START_REQUEST 0x20
#define CRC_CAL_INIT_REQUEST   0x21
#define CRC_DAQ_INIT_REQUEST   0x22
#define CRC_CODE_REQUEST       0x23

/* C3 (Errors) */
#define CRC_CMD_UNKNOWN        0x30
#define CRC_CMD_SYNTAX         0x31
#define CRC_OUT_OF_RANGE       0x32
#define CRC_ACCESS_DENIED      0x33
#define CRC_OVERLOAD           0x34
#define CRC_ACCESS_LOCKED      0x35


/* Session Status */
#define SS_CAL                0x01
#define SS_DAQ                0x02
#define SS_RESUME             0x04
#define SS_TMP_DISCONNECTED   0x10
#define SS_CONNECTED          0x20
#define SS_STORE              0x40
#define SS_RUN                0x80

/* Priviledge Level */
#define PL_CAL 0x01
#define PL_DAQ 0x02
#define PL_PGM 0x40

/*--------------------------------------------------------------------------*/
/* Datatypes */
/*--------------------------------------------------------------------------*/

/*
The types CCP_BYTE, CCP_WORD and CCP_DWORD should be defined in ccppar.h
*/

#ifndef CCP_BYTE
  #ifdef BYTE
    #define CCP_BYTE BYTE
  #else
    #define CCP_BYTE uint8
  #endif
#endif
#ifndef CCP_WORD
  #ifdef WORD
    #define CCP_WORD WORD
  #else
    #define CCP_WORD uint16
  #endif
#endif
#ifndef CCP_DWORD
  #ifdef DWORD
    #define CCP_DWORD DWORD
  #else
    #define CCP_DWORD uint32
  #endif
#endif
#ifndef CCP_BYTEPTR
  #ifdef BYTEPTR
    #define CCP_BYTEPTR BYTEPTR
  #else
    #define CCP_BYTEPTR P2VAR(uint8, AUTOMATIC, CCP_APPL_DATA)
  #endif
#endif
#ifndef CCP_MTABYTEPTR
  #ifdef MTABYTEPTR
    #define CCP_MTABYTEPTR MTABYTEPTR
  #else
    #if defined ( CCP_ENABLE_MEM_ACCESS_BY_APPL )
      #define CCP_MTABYTEPTR CCP_DWORD
    #else
      #define CCP_MTABYTEPTR P2VAR(CCP_BYTE, AUTOMATIC, CCP_MTA_DATA)
    #endif
  #endif
#endif
#ifndef CCP_DAQBYTEPTR
  #ifdef DAQBYTEPTR
    #define CCP_DAQBYTEPTR DAQBYTEPTR
  #else
    #if defined ( CCP_ENABLE_MEM_ACCESS_BY_APPL )
      #define CCP_DAQBYTEPTR CCP_DWORD
    #else
      #define CCP_DAQBYTEPTR P2VAR(CCP_BYTE, AUTOMATIC, CCP_DAQ_DATA)
    #endif
  #endif
#endif

/* Vector CAN Driver Intel/Motorola */
#if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
  #define CCP_MOTOROLA
#endif
#if ( CPU_BYTE_ORDER == LOW_BYTE_FIRST )
  #define CCP_INTEL
#endif
#ifndef CCP_INTEL
  #ifndef CCP_MOTOROLA
    #error Please #define CCP_INTEL or CCP_MOTOROLA as the target system byte order
  #endif
#endif

#ifndef CCP_CANID_CHECK
# define CCP_CANID_CHECK STD_ON
#endif

/*--------------------------------------------------------------------------*/
/* DAQ Lists, Type Definition */
/*--------------------------------------------------------------------------*/

/* Note:
   - Adressextensions are not used in this example
*/

/* CCP_DAQBYTEPTR and CCP_MTABYTEPTR may be defined different to CCP_BYTEPTR to save memory
   Example:
     #define CCP_BYTEPTR     unsigned char *
     #define CCP_MTABYTEPTR  huge unsigned char *
     #define CCP_DAQBYTEPTR  unsigned char *
*/
#ifndef CCP_DAQBYTEPTR
# define CCP_DAQBYTEPTR CCP_BYTEPTR
#endif
#ifndef CCP_MTABYTEPTR
# define CCP_MTABYTEPTR CCP_BYTEPTR
#endif


#if ( CCP_USE_DAQ == STD_ON )

/* ODT entry */
typedef struct ccpOdtEntry
{
  CCP_DAQBYTEPTR ptr;
  CCP_BYTE       siz;
} ccpOdtEntry_t;

/* ODT */
typedef ccpOdtEntry_t ccpODT_t[7];

/* DAQ list */
#define DAQ_FLAG_START    0x01
#define DAQ_FLAG_SEND     0x02
#define DAQ_FLAG_PREPARED 0x04
#define DAQ_FLAG_OVERRUN  0x80
typedef struct ccpDaqList
{
  ccpODT_t        odt[Ccp_GetMaxOdt()];
  CCP_WORD        prescaler;
  CCP_WORD        cycle;
  CCP_BYTE        eventChannel;
  CCP_BYTE        last;
  CCP_BYTE        flags;
} ccpDaqList_t;

#else
#endif

/*--------------------------------------------------------------------------*/
/* Transmit Queue, Type Definition */
/*--------------------------------------------------------------------------*/
#if ( CCP_TRANSMIT_QUEUE == STD_ON )

/* Checks */

#if Ccp_GetMaxOdt() > Ccp_GetTransmitQueueSize()
  #error CCP_SEND_QUEUE_SIZE is too small
#endif


typedef struct
{
  CCP_BYTE b[8];
} ccpMsg_t;

typedef struct
{
  CCP_BYTE     len;
  CCP_BYTE     rp;
  ccpMsg_t     msg[Ccp_GetTransmitQueueSize()];
} ccpQueue_t;

#endif

/*--------------------------------------------------------------------------*/
/* CCP Driver Variables, Type Definition */
/*--------------------------------------------------------------------------*/

#define CCP_MAX_MTA 3u
#define CCP_INTERNAL_MTA (CCP_MAX_MTA-1u)

/* Return values for ccpWriteMTA and Ccp_CheckWriteEEPROM */
#define CCP_WRITE_DENIED  0
#define CCP_WRITE_OK      1
#define CCP_WRITE_PENDING 2
#define CCP_WRITE_ERROR   3

#define CCP_READ_DENIED   0
#define CCP_READ_OK       1

/* Bitmasks for ccp.SendStatus */
#define CCP_CRM_REQUEST  0x01
#define CCP_DTM_REQUEST  0x02
#define CCP_USR_REQUEST  0x04
#define CCP_CMD_PENDING  0x08
#define CCP_CRM_PENDING  0x10
#define CCP_DTM_PENDING  0x20
#define CCP_USR_PENDING  0x40
#define CCP_TX_PENDING   0x80
#define CCP_SEND_PENDING (CCP_DTM_PENDING|CCP_CRM_PENDING|CCP_USR_PENDING)

struct ccps
{
  CCP_BYTE Crm[8];                           /* CRM Command Return Message buffer */

  CCP_BYTE SessionStatus;
  CCP_BYTE SendStatus;

  CCP_MTABYTEPTR MTA[CCP_MAX_MTA];           /* Memory Transfer Address */

  #if ( CCP_USE_DAQ == STD_ON )

    #if ( CCP_TRANSMIT_QUEUE == STD_ON )
      ccpQueue_t Queue;
    #else
      CCP_BYTE Dtm[8];                       /* DTM Data Transmission Message buffer */
    #endif

    P2VAR(ccpOdtEntry_t, AUTOMATIC, CCP_DAQ_DATA) DaqListPtr;               /* Pointer for SET_DAQ_PTR, make it near to save RAM  */

    ccpDaqList_t   DaqList[Ccp_GetMaxDaq()];     /* DAQ list */

  #endif

  CCP_BYTE UserSessionStatus;                /* Used for GET/SET_SESSION_STATUS */
};

#define CCP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

extern VAR(struct ccps, CCP_VAR_NOINIT) ccp;

#define CCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"


#define CCP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern P2CONST(Ccp_ConfigType, CCP_VAR_NOINIT, CCP_PBCFG) Ccp_ConfigDataPtr;

#define CCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#if( CCP_CONTROL == STD_ON )
#define CCP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(uint8, CCP_VAR_ZERO_INIT) Ccp_ControlDeniedCount;

#define CCP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

#if( CCP_CONTROL == STD_ON )
# define CCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(uint8, CCP_VAR_INIT) Ccp_ControlState;

# define CCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif


/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/

#define CCP_START_SEC_CODE
#include "MemMap.h"

/* Functions from ccp.c */
/*----------------------*/

/* Initialize */
extern FUNC(void, CCP_CODE) Ccp_InitMemory( void );
extern FUNC(void, CCP_CODE) Ccp_Reset( void );
extern FUNC(void, CCP_CODE) Ccp_Init( P2CONST(Ccp_ConfigType, AUTOMATIC, CCP_PBCFG) ConfigPtr );

/* DAQ processor */
extern FUNC(void, CCP_CODE) Ccp_Daq( CCP_BYTE eventChannel );

/* Command processor */
extern FUNC(void, CCP_CODE) Ccp_Command( CCP_BYTEPTR com );

/* Transmit Notification */
/* Returns 0 when the CCP driver is idle */
extern FUNC(CCP_BYTE, CCP_CODE) Ccp_SendCallBack( void );

/* Send a pending CRM */
FUNC(void, CCP_CODE) Ccp_SendCrm( void );


/* Functions provided externally */
/*-------------------------------*/

/* Initialize RAM variables */
extern FUNC(void, CCP_CODE) Ccp_CanInitMemory( void );
extern FUNC(void, CCP_CODE) Ccp_CanInit( void );

/* Returns current active CAN Id */
extern FUNC(uint32, CCP_CODE) Ccp_CanGetCurrentCANId( void );

/* Transmit a message */
/* Returns 1 (TRUE) when successfull */
extern FUNC(void, CCP_CODE) Ccp_Send( CCP_BYTEPTR msg );

/* Background calculation */
extern FUNC(void, CCP_CODE) Ccp_UserMainFunction( void );

/* Generate a pointer from addr_ext and addr */
#if !defined(Ccp_GetPointer) /* Not defined as macro */
  extern FUNC(CCP_MTABYTEPTR, CCP_CODE) Ccp_GetPointer( CCP_BYTE addr_ext, CCP_DWORD addr );
#endif

#if defined ( CCP_ENABLE_MEM_ACCESS_BY_APPL )
  extern FUNC(CCP_BYTE, CCP_CODE) Ccp_Read( CCP_DWORD addr );
  extern FUNC(void, CCP_CODE) Ccp_Write( CCP_DWORD addr, CCP_BYTE data );
#endif

/* Check addresses for valid write access */
/* Returns 0 (false) if access denied */
/* Used only, if write protection of memory areas is required */
#if ( CCP_MEM_PROTECTION == STD_ON )
  #ifndef Ccp_CheckWriteAccess /* Not defined as macro */
    extern FUNC(CCP_BYTE, CCP_CODE) Ccp_CheckWriteAccess( CCP_MTABYTEPTR a, CCP_BYTE size );
  #endif
  #ifndef Ccp_CheckReadAccess /* Not defined as macro */
    extern FUNC(CCP_BYTE, CCP_CODE) Ccp_CheckReadAccess( CCP_MTABYTEPTR a, CCP_BYTE size );
  #endif
#endif

extern FUNC(void, CCP_CODE) Ccp_RxIndication
(
  PduIdType CanCanCcpRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, CCP_APPL_VAR) PduInfoPtr
);

extern FUNC(void, CCP_CODE) Ccp_TxConfirmation( PduIdType CanTxPduId );

#if ( CCP_VERSION_INFO_API == STD_ON )
extern FUNC(void, CCP_CODE) Ccp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CCP_APPL_DATA) CcpVerInfoPtr);
#endif

#endif

#define CCP_STOP_SEC_CODE
#include "MemMap.h"

#endif
