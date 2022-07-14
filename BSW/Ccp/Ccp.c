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
|    File Name:   Ccp.c
|
|  Description: 
|   Implements the CCP module.
|   Used by the CANape CAN Calibration Tool.
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

/* CCP Definitions and Parameters */
#include "Ccp.h"

#if ( CCP_DEV_ERROR_DETECT == STD_ON )
  #include "Det.h"              /* Include Diagnostic Event Manager */
#endif


#ifndef C_DISABLE_CCP
/******************************************************************************/
/* Version check                                                              */
/******************************************************************************/
#if ( CP_ASR4CCP_VERSION != 0x0300 )
  #error "Source and Header file of CCP-Module inconsistent!"
#endif
#if ( CP_ASR4CCP_RELEASE_VERSION != 0x00u )
  #error "Source and Header file of CCP-Module inconsistent!"
#endif

#if( CCP_DRIVER_VERSION > 255)
 #error "Version decreased in CCP-Module"
#endif

/*--------------------------------------------------------------------------*/
/* Memory access */

#if defined ( CCP_ENABLE_MEM_ACCESS_BY_APPL )
  #define ccpReadByte(b)           Ccp_Read( (CCP_DWORD)(b) ) /* PRQA S 3453 */ /*  MD_MSR_19.7 */
  #define ccpWriteByte(dest, src)  Ccp_Write( (CCP_DWORD)(dest), (src) ) /* PRQA S 3453 */ /*  MD_MSR_19.7 */
#else
  #define ccpReadByte(b)           *(b) /* PRQA S 3453 */ /*  MD_MSR_19.7 */
  #define ccpWriteByte(dest, src)  *(dest) = (src) /* PRQA S 3453 */ /*  MD_MSR_19.7 */
#endif

#if ( CCP_DEV_ERROR_DETECT == STD_ON )
/* If DET is enabled, a MISRA-C:2004 analysis will yield deviations to the rules:
*  - 14.3 Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment provided that the first character following the null statement is a white-space character.
*  - 14.7 A function shall have a single point of exit at the end of the function
*  - 19.4 C macros shall only expand to a braced initializer, a constant, a parenthesized expression, a type qualifier, a storage class specifier, or a do-while-zero construct
*  - 19.7 A function should be used in preference to a function-like macro.
*  These deviations are caused by design of the runtime/ressource optimized DET handling and are globally justified.
*/
#define Ccp_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE ) { if(!(CONDITION)) { \
  (void)Det_ReportError( CCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)); return; } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define Ccp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) { if(!(CONDITION)) { \
  (void)Det_ReportError( CCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)); return (RET_VAL); } } /* PRQA S 3458 */ /* MD_MSR_19.4, MD_MSR_19.7 */
#define Ccp_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE ) { if(!(CONDITION)) { \
  (void)Det_ReportError( CCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)); } } /* PRQA S 3458, 3453 */ /* MD_MSR_19.4 */
#define Ccp_CallDetReportError( API_ID, ERROR_CODE ) (void)Det_ReportError( CCP_MODULE_ID, 0, (API_ID),(ERROR_CODE)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
#define Ccp_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE )
#define Ccp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) 
#define Ccp_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE )
#define Ccp_CallDetReportError( API_ID, ERROR_CODE )
#endif

#ifdef STATIC
#else
# define STATIC static
#endif


/*--------------------------------------------------------------------------*/
/* ROM */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* RAM */
/*--------------------------------------------------------------------------*/
/*
   The following structure containes all RAM locations needed by the CCP driver
   It has to be word aligned on a C167 !!!
*/
#define CCP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(struct ccps, CCP_VAR_NOINIT) ccp;

#define CCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define CCP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC volatile VAR(uint8, CCP_VAR_ZERO_INIT) Ccp_InitializationState = CCP_UNINIT;

#if( CCP_CONTROL == STD_ON )
VAR(uint8, CCP_VAR_ZERO_INIT) Ccp_ControlDeniedCount = 0u; /* PRQA S 3408 */ /* MD_Ccp_3408 */
#endif

#define CCP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define CCP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

P2CONST(Ccp_ConfigType, CCP_VAR_NOINIT, CCP_PBCFG) Ccp_ConfigDataPtr;

#define CCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#if( CCP_CONTROL == STD_ON )
# define CCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(uint8, CCP_VAR_INIT) Ccp_ControlState = kCcp_Control_Enable;

# define CCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif


/*--------------------------------------------------------------------------*/
/* CODE */
/*--------------------------------------------------------------------------*/
#define CCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_WriteMTA( CCP_BYTE n, CCP_BYTE size, CCP_BYTEPTR d );
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_ReadMTA( CCP_BYTE n, CCP_BYTE size, CCP_BYTEPTR d );
#if ( CCP_TRANSMIT_QUEUE == STD_ON )
STATIC FUNC(void, CCP_CODE) Ccp_QueueInit(void);
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_QueueWrite(P2VAR(ccpMsg_t, AUTOMATIC, CCP_APPL_DATA) msg); /* PRQA S 3673 */ /* MD_Ccp_3673 */
#endif

#if ( CCP_USE_DAQ == STD_ON )
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_ClearDaqList( CCP_BYTE daq );
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_PrepareDaq(  CCP_BYTE daq, CCP_BYTE last, CCP_BYTE eventChannel, CCP_WORD prescaler );
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_StartDaq( CCP_BYTE daq );
STATIC FUNC(void, CCP_CODE) Ccp_StartAllPreparedDaq(void);
STATIC FUNC(void, CCP_CODE) Ccp_StopDaq ( CCP_BYTE daq );
STATIC FUNC(void, CCP_CODE) Ccp_StopAllDaq( void );
# if ( CCP_TRANSMIT_QUEUE != STD_ON )
    STATIC FUNC(void, CCP_CODE) Ccp_SendDtm( void );
# endif
STATIC FUNC(uint32, CCP_CODE) Ccp_SampleAndTransmitDtm( CCP_BYTE pid, CCP_BYTE daq, CCP_BYTE odt );
#endif


/*--------------------------------------------------------------------------*/
/* Transmit */
/*--------------------------------------------------------------------------*/

/*******************************************************************************
** Function:    Ccp_SendCrm
**
** Purpose:     Send a CRM, if no other message is pending
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_SendCrm( void )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  Ccp_CheckDetErrorReturnVoid( CCP_UNINIT != Ccp_InitializationState,
                               CCP_SENDCRM_SERVICE_ID, CCP_E_NOT_INITIALIZED )
  CCP_DISABLE_INTERRUPT();
  if (0u != (ccp.SendStatus&CCP_SEND_PENDING))
  {
    ccp.SendStatus |= CCP_CRM_REQUEST;
  }
  else
  {
    ccp.SendStatus |= CCP_CRM_PENDING;
    Ccp_Send(ccp.Crm);
  }
  CCP_ENABLE_INTERRUPT();
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/* Send a DTM, if no other message is pending */
#if ( CCP_USE_DAQ == STD_ON )
# if ( CCP_TRANSMIT_QUEUE != STD_ON )
STATIC FUNC(void, CCP_CODE) Ccp_SendDtm( void )
{
  CCP_DISABLE_INTERRUPT();

  if (0u != (ccp.SendStatus&CCP_SEND_PENDING))
  {
    ccp.SendStatus |= CCP_DTM_REQUEST;
  }
  else
  {
    ccp.SendStatus |= CCP_DTM_PENDING;
    Ccp_Send(ccp.Dtm);
  }

  CCP_ENABLE_INTERRUPT();
}
# endif
#endif


/*--------------------------------------------------------------------------*/
/* Transmit Queue */
/*--------------------------------------------------------------------------*/
#if ( CCP_TRANSMIT_QUEUE == STD_ON )
/*******************************************************************************
** Function:    Ccp_QueueInit
**
** Purpose:     Resets send queue pointer/counter
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
STATIC FUNC(void, CCP_CODE) Ccp_QueueInit(void)
{
  Ccp_CheckDetErrorReturnVoid( CCP_UNINIT != Ccp_InitializationState,
                               CCP_QUEUEINIT_SERVICE_ID, CCP_E_NOT_INITIALIZED )

  ccp.Queue.len = 0;
  ccp.Queue.rp = 0;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*******************************************************************************
** Function:    Ccp_QueueWrite
**
** Purpose:     Inserts a message in the send queue
**
**  Parameter:  msg : frame to insert into send queue
**
**  Returncode: 0 if not successful otherwise 1
*******************************************************************************/
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_QueueWrite(P2VAR(ccpMsg_t, AUTOMATIC, CCP_APPL_DATA) msg) /* PRQA S 3673 */ /* MD_Ccp_3673 */
{
  Ccp_CheckDetErrorReturnValue( CCP_UNINIT != Ccp_InitializationState,
                                CCP_QUEUEWRITE_SERVICE_ID, CCP_E_NOT_INITIALIZED, 0 )
  Ccp_CheckDetErrorReturnValue( msg != NULL_PTR,
                                CCP_QUEUEWRITE_SERVICE_ID, CCP_E_NOT_INITIALIZED, CCP_E_NULL_POINTER )

  if (ccp.Queue.len >= Ccp_GetTransmitQueueSize())
  {
    return 0;
  }
  ccp.Queue.msg[(ccp.Queue.rp+ccp.Queue.len) % Ccp_GetTransmitQueueSize()] = *msg;
  ccp.Queue.len++;
  return 1;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif


/*--------------------------------------------------------------------------*/
/* Handle MTAs (Memory-Transfer-Address) */
/*--------------------------------------------------------------------------*/

/* Assign a pointer to a MTA */
#define ccpSetMTA(n,p) ccp.MTA[n] = (p); /* PRQA S 3412 */ /* MD_MSR_19.4 */

/*******************************************************************************
** Function:    Ccp_WriteMTA
**
** Purpose:     Write n bytes
**
**  Parameter:  n : nta Pointer index
**              size : number of bytes to write
**              d : Pointer to source data
**
**  Returncode: status information
*******************************************************************************/
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_WriteMTA( CCP_BYTE n, CCP_BYTE size, CCP_BYTEPTR d ) /* PRQA S 3673 */ /* MD_Ccp_3673 */
{
  /* Checked ram memory write access */
#if ( CCP_MEM_PROTECTION == STD_ON )
    if (!Ccp_CheckWriteAccess(ccp.MTA[n],size))
    {
      ccp.MTA[n] += size; /* PRQA S 0488 */ /* MD_Ccp_0488 */
      return CCP_WRITE_DENIED;
    }
#endif

   while (size-->0) /* PRQA S 3440 */ /* MD_Ccp_3440 */
   {
     /* COSMIC Compiler Bug: *(ccp.MTA[n]++) = *(d++);  */
     ccpWriteByte( ccp.MTA[n], *d );
     ccp.MTA[n]++; /* PRQA S 0489 */ /* MD_Ccp_0489 */
     d++; /* PRQA S 0489 */ /* MD_Ccp_0489 */
   }
   return CCP_WRITE_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_WriteMTA
**
** Purpose:     Read n bytes
**
**  Parameter:  n : nta Pointer index
**              size : number of bytes to write
**              d : Pointer to destination
**
**  Returncode: -
*******************************************************************************/
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_ReadMTA( CCP_BYTE n, CCP_BYTE size, CCP_BYTEPTR d )
{
#if ( CCP_MEM_PROTECTION == STD_ON )
    if (!Ccp_CheckReadAccess(ccp.MTA[n],size))
    {
      ccp.MTA[n] += size; /* PRQA S 0488 */ /* MD_Ccp_0488 */
      return CCP_READ_DENIED;
    }
#endif

  while (size-- > 0) /* PRQA S 3440 */ /* MD_Ccp_3440 */
  {
    *d = ccpReadByte( ccp.MTA[n] );
    d++; /* PRQA S 0489 */ /* MD_Ccp_0489 */
                        /* !! Attention: if the emulator stops here due to an unknown TRAP !! */
                        /* !!            the ECU format setting (Motorola/Intel)           !! */
                        /* !!            in your MCD (e.g. CANape) is not correct.         !! */
    ccp.MTA[n]++; /* PRQA S 0489 */ /* MD_Ccp_0489 */
  }
  return CCP_READ_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*--------------------------------------------------------------------------*/
/* Data Aquisition Setup */
/*--------------------------------------------------------------------------*/

#if ( CCP_USE_DAQ == STD_ON )

/*******************************************************************************
** Function:    Ccp_ClearDaqList
**
** Purpose:     Clear DAQ list
**
**  Parameter:  daq : daq list number
**
**  Returncode: CCP_MAX_ODT for daq list
*******************************************************************************/
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_ClearDaqList( CCP_BYTE daq )
{
  CCP_BYTEPTR p;
  CCP_BYTEPTR pl;

  if (daq >= Ccp_GetMaxDaq())
  {
    return 0;
  }

  /* Clear this daq list to zero */
  p = (CCP_BYTEPTR)&ccp.DaqList[daq];/* PRQA S 0310 */ /* Cast to lesser alignment */
  pl = p+sizeof(ccpDaqList_t); /* PRQA S 0488 */ /* MD_Ccp_0488 */
  while (p<pl)
  {
    *p++ = 0; /* PRQA S 3440, 0489 */ /* MD_Ccp_3440 */ /* MD_Ccp_0489 */
  }

  /* Not DAQ list specific */
  ccp.SessionStatus |= SS_DAQ;
#if ( CCP_TRANSMIT_QUEUE == STD_ON )
  Ccp_QueueInit();
#endif

  return Ccp_GetMaxOdt();
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_PrepareDaq
**
** Purpose:     Prepare DAQ
**
**  Parameter:  daq : daq list number
**              last : last odt of daq
**              eventChannel : assigned event channel number
**              prescaler : prescaler of daq list
**
**  Returncode: 1 if successful
*******************************************************************************/
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_PrepareDaq(  CCP_BYTE daq, CCP_BYTE last, CCP_BYTE eventChannel, CCP_WORD prescaler )
{
  if (daq >= Ccp_GetMaxDaq())
  {
    return 0;
  }

  ccp.DaqList[daq].eventChannel = eventChannel;
  if (prescaler==0)
  {
    prescaler = 1;
  }
  ccp.DaqList[daq].prescaler = prescaler;
  ccp.DaqList[daq].cycle = 1;
  ccp.DaqList[daq].last = last;
  ccp.DaqList[daq].flags = DAQ_FLAG_PREPARED;
  return 1;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_StartDaq
**
** Purpose:     Start DAQ
**
**  Parameter:  daq : daq list number
**
**  Returncode: 1 if successful
*******************************************************************************/
STATIC FUNC(CCP_BYTE, CCP_CODE) Ccp_StartDaq( CCP_BYTE daq )
{
  if (daq >= Ccp_GetMaxDaq())
  {
    return 0;
  }

  ccp.DaqList[daq].flags = DAQ_FLAG_START;
  ccp.SessionStatus |= (uint8)SS_RUN;

  return 1;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_StartAllPreparedDaq
**
** Purpose:     Start all prepared DAQs
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
STATIC FUNC(void, CCP_CODE) Ccp_StartAllPreparedDaq(void)
{
  CCP_BYTE q;

  for (q=0; q<Ccp_GetMaxDaq(); q++)
  {
    if (ccp.DaqList[q].flags==DAQ_FLAG_PREPARED)
    {
      ccp.DaqList[q].flags = DAQ_FLAG_START;
    }
  }
  ccp.SessionStatus |= (uint8)SS_RUN;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_StopDaq
**
** Purpose:     Stop DAQ
**
**  Parameter:  daq : daq list number
**
**  Returncode: -
*******************************************************************************/
STATIC FUNC(void, CCP_CODE) Ccp_StopDaq ( CCP_BYTE daq )
{
  CCP_BYTE i;

  if (daq >= Ccp_GetMaxDaq())
  {
    return;
  }
  ccp.DaqList[daq].flags = 0;

  /* check if all DAQ lists are stopped */
  for (i=0; i<Ccp_GetMaxDaq(); i++)
  {
    if (DAQ_FLAG_START == (ccp.DaqList[i].flags&DAQ_FLAG_START))
    {
      return;
    }
  }

  ccp.SessionStatus &= (CCP_BYTE)(~SS_RUN); /* PRQA S 0277 */ /* MD_Ccp_0277 */
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_StopAllDaq
**
** Purpose:     Stop all DAQ
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
STATIC FUNC(void, CCP_CODE) Ccp_StopAllDaq( void )
{
  CCP_BYTE q;

  for (q=0; q<Ccp_GetMaxDaq(); q++)
  {
    ccp.DaqList[q].flags = 0;
  }
  ccp.SessionStatus &= (CCP_BYTE)(~SS_RUN); /* PRQA S 0277 */ /* MD_Ccp_0277 */
}


/*--------------------------------------------------------------------------*/
/* Data Aquisition Processor */
/*--------------------------------------------------------------------------*/

/*******************************************************************************
** Function:    Ccp_SampleAndTransmitDtm
**
** Purpose:     Sample and transmit a DTM
**
**  Parameter:  pid : pid of odt
**              daq : daq list number
**              odt : odt number
**
**  Returncode: 1 if successful
*******************************************************************************/
STATIC FUNC(uint32, CCP_CODE) Ccp_SampleAndTransmitDtm( CCP_BYTE pid, CCP_BYTE daq, CCP_BYTE odt )
{
#if ( CCP_TRANSMIT_QUEUE == STD_ON )
    CCP_BYTE dtm[8];
#else
    #define dtm ccp.Dtm /* PRQA S 0842 */ /* MD_Ccp_0842 */
#endif
    CCP_DWORD p;

    CCP_BYTE s;
    P2VAR(CCP_BYTE, AUTOMATIC, CCP_APPL_DATA) d;
    P2VAR(CCP_BYTE, AUTOMATIC, CCP_APPL_DATA) dl;
    P2VAR(ccpOdtEntry_t, AUTOMATIC, CCP_APPL_DATA) e;
    P2VAR(ccpOdtEntry_t, AUTOMATIC, CCP_APPL_DATA) el;
    sint8 i;

  /* PID */
  dtm[0] = pid;

  /* Assure data consistency */
  CCP_DISABLE_INTERRUPT();

  /* Sample */
    e = (P2VAR(ccpOdtEntry_t, AUTOMATIC, CCP_APPL_DATA))&ccp.DaqList[daq].odt[odt][0];
    el = e+8; /* PRQA S 0488 */ /* MD_Ccp_0488 */
    d = &dtm[1];
    dl = d+7; /* PRQA S 0488 */ /* MD_Ccp_0488 */
    /* ESCAN00093488 */
    while (((d+(e->siz))<=dl) && (e<el) && (e->ptr)) /* PRQA S 0488 */ /* MD_Ccp_0488 */
    {
      p = (CCP_DWORD)e->ptr; /* PRQA S 0306 */ /* MD_Ccp_0306 */
      s = e->siz;
      if (s==4)
      {
    #if defined ( CCP_ENABLE_UNALIGNED_MEM_ACCESS )
        *(P2VAR(CCP_DWORD, AUTOMATIC, CCP_APPL_VAR))d = *(P2VAR(CCP_DWORD, AUTOMATIC, CCP_APPL_VAR))p;
    #else
        i = 3;
        while( i >= 0 )
        {
          *d = ccpReadByte((P2VAR(CCP_BYTE, AUTOMATIC, CCP_APPL_DATA))p); /* PRQA S 0306 */ /* MD_Ccp_0306 */
           d++; /* PRQA S 0489 */ /* MD_Ccp_0489 */
           p++;
           i--;
        }
    #endif
      }
      else
      if (s==2)
      {
    #if defined ( CCP_ENABLE_UNALIGNED_MEM_ACCESS )
        *(P2VAR(CCP_WORD, AUTOMATIC, CCP_APPL_VAR))d = *(P2VAR(CCP_WORD, AUTOMATIC, CCP_APPL_VAR))p;
    #else
        i = 1;

        while( i >= 0 )
        {
          *d = ccpReadByte((P2VAR(CCP_BYTE, AUTOMATIC, CCP_APPL_DATA))p); /* PRQA S 0306 */ /* MD_Ccp_0306 */
          d++; /* PRQA S 0489 */ /* MD_Ccp_0489 */
          p++;
          i--;
        }
    #endif
      }
      else
      {
        *d = ccpReadByte((P2VAR(CCP_BYTE, AUTOMATIC, CCP_APPL_DATA))p); /* PRQA S 0306 */ /* MD_Ccp_0306 */
        d++; /* PRQA S 0489 */ /* MD_Ccp_0489 */
      }
      e++; /* PRQA S 0489 */ /* MD_Ccp_0489 */
    }

  /* Queue or transmit the DTM */
#if ( CCP_TRANSMIT_QUEUE == STD_ON )
    if (0u != (ccp.SendStatus&CCP_SEND_PENDING))
    {
      if (!Ccp_QueueWrite((P2VAR(ccpMsg_t, AUTOMATIC, CCP_APPL_DATA))dtm)) /* PRQA S 0310 */ /* MD_Ccp_0310 */
      {
        /* Overun */
        CCP_ENABLE_INTERRUPT();
        return 0;
      }
    }
    else
    {
      ccp.SendStatus |= CCP_DTM_PENDING;
      Ccp_Send(dtm);
    }

#else

    if (0 != (ccp.SendStatus&CCP_DTM_REQUEST))
    {
      /* Overun */
      CCP_ENABLE_INTERRUPT();
      return 0;
    }
    if (0 != (ccp.SendStatus&CCP_SEND_PENDING))
    {
      ccp.SendStatus |= CCP_DTM_REQUEST;
    }
    else
    {
      ccp.SendStatus |= CCP_DTM_PENDING;
      Ccp_Send(dtm);
    }
#endif

  CCP_ENABLE_INTERRUPT();
  return 1;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */



/*******************************************************************************
** Function:    Ccp_Daq
**
** Purpose:     Data aquisition
**
**  Parameter:  eventChannel : event channel number
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_Daq( CCP_BYTE eventChannel )
{
  CCP_BYTE q,o;
  CCP_BYTE j;

#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  Ccp_CheckDetErrorReturnVoid( CCP_UNINIT != Ccp_InitializationState,
                               CCP_DAQ_SERVICE_ID, CCP_E_NOT_INITIALIZED )

  if (0 == (ccp.SessionStatus&SS_RUN))
  {
    return;
  }

  for (o=0,q=0; q<Ccp_GetMaxDaq(); o+=Ccp_GetMaxOdt(),q++) /* PRQA S 3418 */ /* MD_Ccp_3418 */
  {
    if (0 == (ccp.DaqList[q].flags&DAQ_FLAG_START))
    {
      continue; /* PRQA S 0770 */ /* MD_Ccp_0770 */
    }
    if (ccp.DaqList[q].eventChannel != eventChannel)
    {
      continue; /* PRQA S 0770 */ /* MD_Ccp_0770 */
    }
    if (--ccp.DaqList[q].cycle != 0) /* PRQA S 3440 */ /* MD_Ccp_3440 */
    {
      continue; /* PRQA S 0770 */ /* MD_Ccp_0770 */
    }
    ccp.DaqList[q].cycle = ccp.DaqList[q].prescaler;

    /* Check that the current queue space fits a complete cycle */
    #if (CCP_TRANSMIT_QUEUE == STD_ON) && (CCP_OVERRUN_DETECTION == STD_ON)
      if ((Ccp_GetTransmitQueueSize()-ccp.Queue.len) <= ccp.DaqList[q].last)
      {
        ccp.DaqList[q].flags |= (CCP_BYTE)DAQ_FLAG_OVERRUN;
        /* Skip this DAQ list on overrun */
        continue; /* PRQA S 0770 */ /* MD_Ccp_0770 */
      }
    #endif

    /* Use BIT7 of PID to indicate overruns (CANape special feature) */
    #if (CCP_OVERRUN_DETECTION == STD_ON)
      for (j=0;j<=ccp.DaqList[q].last;j++)
      {
        if (!Ccp_SampleAndTransmitDtm((CCP_BYTE)((o+j)|(ccp.DaqList[q].flags & (CCP_BYTE)DAQ_FLAG_OVERRUN)),q,j))
        {
          ccp.DaqList[q].flags |= (CCP_BYTE)DAQ_FLAG_OVERRUN;
        }
        else
        {
          ccp.DaqList[q].flags &= (CCP_BYTE)(~DAQ_FLAG_OVERRUN); /* PRQA S 0277 */ /* MD_Ccp_0277 */
        }
      } /* j */
    #else
      for (j=0;j<=ccp.DaqList[q].last;j++)
      {
        (void)Ccp_SampleAndTransmitDtm(o+j,q,j);
      } /* j */

    #endif
  } /* q */

  /* Check for the next ODT to send */
} /* PRQA S 2006, 6010 */ /* MD_MSR_14.7 */ /* MD_MSR_STPTH */
#endif /* CCP_DAQ */


/*--------------------------------------------------------------------------*/
/* Background Processing */
/*--------------------------------------------------------------------------*/


/*******************************************************************************
** Function:    Ccp_MainFunction
**
** Purpose:     cyclic main function
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_MainFunction( void )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  if( CCP_UNINIT != Ccp_InitializationState )
  {
    /* Call the user background function */
    Ccp_UserMainFunction();
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*--------------------------------------------------------------------------*/
/* Command Processor */
/*--------------------------------------------------------------------------*/

/*******************************************************************************
** Function:    Ccp_Command
**
** Purpose:     command processor
**
**  Parameter:  com : pointer to command string
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_Command( CCP_BYTEPTR com )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  Ccp_CheckDetErrorReturnVoid( CCP_UNINIT != Ccp_InitializationState,
                               CCP_COMMAND_SERVICE_ID, CCP_E_NOT_INITIALIZED )
  Ccp_CheckDetErrorReturnVoid( NULL_PTR != com,
                               CCP_COMMAND_SERVICE_ID, CCP_E_NULL_POINTER )

  #define cmd com[0] /* PRQA S 0842 */ /* MD_Ccp_0842 */
  #define ctr com[1] /* PRQA S 0842 */ /* MD_Ccp_0842 */

  /* Handle CONNECT or TEST command */
  if ((cmd==CC_CONNECT) || (cmd==CC_TEST))
  {
    uint16 stationAddr;
    /* Has to be Intel-Format ! */
    stationAddr = ((uint16)com[3] << 8) | com[2];

    /* This station */
    if ((stationAddr==(uint16)Ccp_GetStationAddress())
    #if ( CCP_BROADCAST_STATION_ADDRESS == STD_ON )
      || (stationAddr==(uint16)Ccp_GetBroadcastStationAddress())
    #endif
      ) /* PRQA S 0310, 3305 */ /* MD_Ccp_0310 */ /* MD_Ccp_3305 */
    { /* This station */
      if (cmd==CC_CONNECT)
      {
        #if ( CCP_USE_DAQ == STD_ON )
          if (0 == (ccp.SessionStatus&SS_TMP_DISCONNECTED))
          {
            Ccp_StopAllDaq();
            ccp.SendStatus = 0; /* Clear all transmission flags */
          }
        #endif
        ccp.SessionStatus |= (CCP_BYTE)(SS_CONNECTED);
        ccp.SessionStatus &= (CCP_BYTE)(~SS_TMP_DISCONNECTED); /* PRQA S 0277 */ /* MD_Ccp_0277 */
      }

      /* Response */
      /* Station addresses in Intel Format */
      ccp.Crm[0] = 0xFF;
      ccp.Crm[1] = CRC_OK;
      ccp.Crm[2] = ctr;
      ccp.Crm[3] = 0xFE;
      ccp.Crm[4] = (uint8)((uint16)Ccp_GetStationAddress() & 0xff);
      ccp.Crm[5] = (uint8)((uint16)Ccp_GetStationAddress() >> 8);
      #if ( CCP_BROADCAST_STATION_ADDRESS == STD_ON )
        ccp.Crm[6] = (uint8)((uint16)Ccp_GetBroadcastStationAddress() & 0xff);
        ccp.Crm[7] = (uint8)((uint16)Ccp_GetBroadcastStationAddress() >> 8);
      #else
        *(P2VAR(CCP_WORD, AUTOMATIC, CCP_APPL_VAR))&ccp.Crm[6] = 0x0000; /* PRQA S 0310 */ /* MD_Ccp_0310 */
      #endif

      /* response */
      /* |||| */
    }
    /* Another station */
    else
    {
      /* If connected, temporary disconnect */
      if (SS_CONNECTED == (ccp.SessionStatus&SS_CONNECTED))
      {
        ccp.SessionStatus &= (CCP_BYTE)(~SS_CONNECTED); /* PRQA S 0277 */ /* MD_Ccp_0277 */
        ccp.SessionStatus |= (CCP_BYTE)(SS_TMP_DISCONNECTED);
      }
      /* no responce */
      return;
    }
  }

  /* Handle other commands only if connected */
  else if (SS_CONNECTED == (ccp.SessionStatus&SS_CONNECTED))
  {
    /* prepare the response */
    ccp.Crm[0] = 0xFF;
    ccp.Crm[1] = CRC_OK;
    ccp.Crm[2] = ctr;

    switch (cmd)
    {
      case CC_DISCONNECT:
        {
          #define disconnectCmd com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          uint16 stationAddr;
          /* Has to be Intel-Format ! */
          stationAddr = ((uint16)com[5] << 8) | com[4];

          if ((stationAddr==(uint16)Ccp_GetStationAddress()) 
          #if ( CCP_BROADCAST_STATION_ADDRESS == STD_ON )
            || (stationAddr==(uint16)Ccp_GetBroadcastStationAddress())
          #endif
            ) /* PRQA S 0310, 3305 */ /* MD_Ccp_0310 */ /* MD_Ccp_3305 */
          {
            ccp.SessionStatus &= (CCP_BYTE)(~SS_CONNECTED); /* PRQA S 0277 */ /* MD_Ccp_0277 */

            if (disconnectCmd==0x00)
            { /* Temporary */
              ccp.SessionStatus |= SS_TMP_DISCONNECTED;
            }
            else
            {           /* End of session */
              #if ( CCP_USE_DAQ == STD_ON )
                Ccp_StopAllDaq();
              #endif
            }
          }
          else
          {
            return;
          }
        }
        break;

      case CC_EXCHANGE_ID: /* Exchange Station Identifications */
        {
          CCP_BYTE i;
          #define masterId com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          for(i=0; Ccp_StationId[i]!=0; i++)
          {
          }
          ccp.Crm[3] = i; /* Lenght of slave device identifier */
          ccp.Crm[4] = 0;
          /* Build the Resource Availability and Protection Mask */
          ccp.Crm[5] = PL_CAL; /* Default: Calibration available */
          ccp.Crm[6] = 0;      /* Default: No Protection */
          #if ( CCP_USE_DAQ == STD_ON )
            ccp.Crm[5] |= PL_DAQ;     /* Data Acquisition */
          #endif
          ccp.Crm[7] = CCP_DRIVER_VERSION; /* Driver version number */
          #if defined ( CCP_ENABLE_MEM_ACCESS_BY_APPL )
          ccpSetMTA(0, Ccp_GetPointer(0xFF, (CCP_MTABYTEPTR)Ccp_StationId)) /* PRQA S 0311 */ /* MD_Ccp_0311 */
          #else
          ccpSetMTA(0,(CCP_MTABYTEPTR)Ccp_StationId) /* PRQA S 0311 */ /* MD_Ccp_0311 */
          #endif
        }
        break;

      case CC_SET_MTA: /* Set transfer address */
        {
          #define mta     com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define addrExt com[3] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define addr    (*(P2VAR(CCP_DWORD, AUTOMATIC, CCP_APPL_VAR))&com[4]) /* PRQA S 0842 */ /* MD_Ccp_0842 */
         if (mta<(CCP_MAX_MTA-1))
         {
            ccpSetMTA(mta,Ccp_GetPointer(addrExt,addr)) /* PRQA S 0310, 3305 */ /* MD_Ccp_0310 */ /* MD_Ccp_3305 */
         }
         else
         {
            ccp.Crm[1] = CRC_OUT_OF_RANGE;
         }

          #undef mta /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef addrExt /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef addr /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      case CC_DNLOAD: /* Download */
        {
          CCP_BYTE r;
          #define size com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */

          if( size < 6 )
          {
            r = Ccp_WriteMTA(0,size,&com[3]);
            if (r==CCP_WRITE_PENDING)
            {
              return; /* EEPROM write pending */
            }
            if ((r==CCP_WRITE_DENIED) || (r==CCP_WRITE_ERROR))
            {
              ccp.Crm[1] = CRC_ACCESS_DENIED; /* No write access */
            }
          }
          else
          {
            ccp.Crm[1] = CRC_OUT_OF_RANGE;
          }
          /* avoid compiler re-definition warning */
          #undef size /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      case CC_DNLOAD6: /* Download */
        {
          CCP_BYTE r;
          r = Ccp_WriteMTA(0,6,&com[2]);
          if (r==CCP_WRITE_PENDING)
          {
            return; /* EEPROM write pending */
          }
          if ((r==CCP_WRITE_DENIED) || (r==CCP_WRITE_ERROR))
          {
            ccp.Crm[1] = CRC_ACCESS_DENIED; /* No write access */
          }
        }
        break;

      case CC_UPLOAD: /* Upload */
        {
          #define size com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          if( size < 6 )
          {
            CCP_BYTE r;
            r = Ccp_ReadMTA(0,size,&ccp.Crm[3]);
            if (r==CCP_READ_DENIED)
            {
              ccp.Crm[1] = CRC_ACCESS_DENIED; /* No read access */
            }
          }
          else
          {
            ccp.Crm[1] = CRC_OUT_OF_RANGE;
          }
          /* avoid compiler re-definition warning */
          #undef size /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      case CC_SHORT_UPLOAD: /* Upload with Address */
        {
          #define size    com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define addrExt com[3] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define addr    (*(P2VAR(CCP_DWORD, AUTOMATIC, CCP_APPL_VAR))&com[4]) /* PRQA S 0842 */ /* MD_Ccp_0842 */

          if( size < 6 )
          {
            CCP_BYTE r;
            ccpSetMTA(CCP_INTERNAL_MTA,Ccp_GetPointer(addrExt,addr)) /* PRQA S 0310, 3305 */ /* MD_Ccp_0310 */ /* MD_Ccp_3305 */
            r = Ccp_ReadMTA((CCP_BYTE)CCP_INTERNAL_MTA,size,&ccp.Crm[3]);
            if (r==CCP_READ_DENIED)
            {
              ccp.Crm[1] = CRC_ACCESS_DENIED; /* No read access */
            }
          }
          else
          {
             ccp.Crm[1] = CRC_OUT_OF_RANGE;
          }

          #undef size /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef addrExt /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef addr /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      case CC_GET_DAQ_SIZE: /* Return the size of a DAQ list and clear */
        {
          #define daqList com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define daqId   (*(P2VAR(CCP_DWORD, AUTOMATIC, CCP_APPL_VAR))&com[4]) /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #if ( CCP_USE_DAQ == STD_ON )
            Ccp_StopDaq(daqList); /* Stop this daq list */
            ccp.Crm[3] = Ccp_ClearDaqList(daqList); /* Number of  ODTs */
            ccp.Crm[4] = (uint8)(daqList * Ccp_GetMaxOdt()); /* PID of the first ODT */
            #if ( CCP_CANID_CHECK == STD_ON )
              /* Remove MSB as it is used to mark extended CAN-Ids */
              if((daqId & 0x7FFFFFFF) != Ccp_CanGetCurrentCANId()) /* PRQA S 0310 */ /* MD_Ccp_0310 */
              {
                ccp.Crm[1] = CRC_OUT_OF_RANGE;
              }
            #endif
          #else
            ccp.Crm[3] = 0;
            ccp.Crm[4] = 0;
          #endif
          #undef daqList /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef daqId /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      #if ( CCP_USE_DAQ == STD_ON )

      case CC_SET_DAQ_PTR: /* Set DAQ pointer */
        {
          #define comDaq  com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define comOdt  com[3] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define comIdx  com[4] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          if ((comDaq >= Ccp_GetMaxDaq()) || (comOdt >= Ccp_GetMaxOdt()) || (comIdx>7))
          {
            ccp.Crm[1] = CRC_OUT_OF_RANGE;
            ccp.DaqListPtr = 0;
          }
          else
          {
            ccp.DaqListPtr = &ccp.DaqList[comDaq].odt[comOdt][comIdx];
          }

          #undef comDaq /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef comOdt /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef comIdx /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      case CC_WRITE_DAQ: /* Write DAQ entry */
        {
          #define writeDaqSize    com[2] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define writeDaqAddrExt com[3] /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define writeDaqAddr    (*(P2VAR(CCP_DWORD, AUTOMATIC, CCP_APPL_VAR))&com[4]) /* PRQA S 0842 */ /* MD_Ccp_0842 */
          if ( ((writeDaqSize!=1) && (writeDaqSize!=2) && (writeDaqSize!=4))
            || (ccp.DaqListPtr==0))
          {
            ccp.Crm[1] = CRC_OUT_OF_RANGE;
          }
          else
          {
#if ( CCP_MEM_PROTECTION == STD_ON )
            if (!Ccp_CheckReadAccess((CCP_MTABYTEPTR)Ccp_GetPointer(writeDaqAddrExt,writeDaqAddr), writeDaqSize)) /* PRQA S 0310 */ /* MD_Ccp_0310 */
            {
              ccp.Crm[1] = CRC_ACCESS_DENIED; /* No read access */
            }
            else
            {
#endif
              ccp.DaqListPtr->ptr = (CCP_DAQBYTEPTR)Ccp_GetPointer(writeDaqAddrExt,writeDaqAddr); /* PRQA S 0310, 3305 */ /* MD_Ccp_0310 */ /* MD_Ccp_3305 */
              ccp.DaqListPtr->siz = writeDaqSize;
#if ( CCP_MEM_PROTECTION == STD_ON )
            }
#endif
          }

          #undef writeDaqSize /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef writeDaqAddrExt /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef writeDaqAddr /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      case CC_START_STOP: /* Cyclic aquisition start/stop */
        {
          #define ssCmd                 com[2]  /* Start or Stop */ /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define ssDaq                 com[3]  /* DAQ list */ /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define ssLast                com[4]  /* Last ODT to send */ /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define ssEventChannel        com[5]  /* Event Channel Number */ /* PRQA S 0842 */ /* MD_Ccp_0842 */
          #define ssPrescaler (*(P2VAR(CCP_WORD, AUTOMATIC, CCP_APPL_VAR))&com[6]) /* Prescaler */ /* PRQA S 0842 */ /* MD_Ccp_0842 */

          if (0 == (ccp.SessionStatus&SS_DAQ))
          { /* Not initialized */
           ccp.Crm[1] = CRC_DAQ_INIT_REQUEST;
          }
          else
          if( (ssLast >= Ccp_GetMaxOdt()) )
          {
            ccp.Crm[1] = CRC_OUT_OF_RANGE;
          }
          else
          if( (ssDaq >= Ccp_GetMaxDaq()) )
          {
            ccp.Crm[1] = CRC_ACCESS_DENIED;
          }
          else
          if( ssPrescaler == 0 ) /* PRQA S 0310 */ /* MD_Ccp_0310 */
          {
            ccp.Crm[1] = CRC_OUT_OF_RANGE;
          }
          else
          {
            switch (ssCmd)
            {
              case 0: /* stop */
                Ccp_StopDaq(ssDaq);
                break;
              case 1: /* start */
                (void)Ccp_PrepareDaq(ssDaq,ssLast,ssEventChannel,ssPrescaler); /* PRQA S 0310, 3305 */ /* MD_Ccp_0310 */ /* MD_Ccp_3305 */
                (void)Ccp_StartDaq(ssDaq);
                break;
              case 2: /* prepare */
                (void)Ccp_PrepareDaq(ssDaq,ssLast,ssEventChannel,ssPrescaler); /* PRQA S 0310, 3305 */ /* MD_Ccp_0310 */ /* MD_Ccp_3305 */
                break;
              default:
                ccp.Crm[1] = CRC_OUT_OF_RANGE;
                break;
            }
          }

          #undef ssCmd /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef ssDaq /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef ssLast /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef ssEventChannel /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
          #undef ssPrescaler /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      case CC_START_STOP_ALL: /* Cyclic aquisition start/stop */
        {
          #define ssCmd com[2]  /* Start or Stop */ /* PRQA S 0842 */ /* MD_Ccp_0842 */

          if (0 == (ccp.SessionStatus&SS_DAQ))
          { /* Not initialized */
           ccp.Crm[1] = CRC_DAQ_INIT_REQUEST;
          }
          else
          {
            switch (ssCmd)
            {
              case 0: /* Stop */
                Ccp_StopAllDaq();
                break;
              case 1: /* Start */
                Ccp_StartAllPreparedDaq();
                break;
              default:
                ccp.Crm[1] = CRC_OUT_OF_RANGE;
                break;
            }
          }

          #undef ssCmd /* PRQA S 0842, 0841 */ /* MD_Ccp_0842 */ /* MD_Ccp_0841 */
        }
        break;

      #endif /* CCP_DAQ */

      case CC_GET_CCP_VERSION: /* Get Version */
        {
          ccp.Crm[3] = CCP_VERSION_MAJOR;
          ccp.Crm[4] = CCP_VERSION_MINOR;
        }
        break;

      default: /* unknown */
        {
          ccp.Crm[1] = CRC_CMD_UNKNOWN;
          break;
        }
    } /* switch */
    /* Responce */
    /* |||| */
  }
  /* Not connected */
  else
  {
    /* No responce */
    return;
  }

  Ccp_SendCrm();
} /* PRQA S 2006, 6010, 6030, 6080, 6050 */ /* MD_MSR_14.7 */ /* MD_MSR_STPTH */ /* MD_MSR_STCYC */ /* MD_MSR_STMIF */ /* MD_MSR_STCAL */


/*--------------------------------------------------------------------------*/
/* Ccp_Send must not fail, when called from this function */
/*--------------------------------------------------------------------------*/
/*******************************************************************************
** Function:    Ccp_SendCallBack
**
** Purpose:     Send notification callback
**
**  Parameter:  -
**
**  Returncode: 1 if successful
*******************************************************************************/
FUNC(CCP_BYTE, CCP_CODE) Ccp_SendCallBack( void )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return 0;
  }
#endif

  Ccp_CheckDetErrorReturnValue( CCP_UNINIT != Ccp_InitializationState,
                                CCP_SEND_CALLBACK_SERVICE_ID, CCP_E_NOT_INITIALIZED, 0 )

  CCP_DISABLE_INTERRUPT();
  /* Clear all pending flags, except for CCP_CMD_PENDING */
  ccp.SendStatus &= (CCP_BYTE)(~CCP_SEND_PENDING); /* PRQA S 0277 */ /* MD_Ccp_0277 */

  /* Send a CRM message */
  if (CCP_CRM_REQUEST == (ccp.SendStatus&CCP_CRM_REQUEST))
  {
    ccp.SendStatus &= (CCP_BYTE)(~CCP_CRM_REQUEST); /* PRQA S 0277 */ /* MD_Ccp_0277 */
    CCP_ENABLE_INTERRUPT();
    Ccp_SendCrm();
    return 1;
  }
  CCP_ENABLE_INTERRUPT();

  /* Send a DAQ message */
  #if ( CCP_USE_DAQ == STD_ON )
    if (SS_RUN == (ccp.SessionStatus&SS_RUN))
    {
      /* Send a  DAQ message (DTM) from the queue */
      #if ( CCP_TRANSMIT_QUEUE == STD_ON )
        CCP_DISABLE_INTERRUPT();
        if (ccp.Queue.len > 0)
        {
          ccp.SendStatus |= CCP_DTM_PENDING;
          Ccp_Send((CCP_BYTEPTR)&ccp.Queue.msg[ccp.Queue.rp]); /* PRQA S 0310 */ /* MD_Ccp_0310 */
          ccp.Queue.rp++;
          if (ccp.Queue.rp >= Ccp_GetTransmitQueueSize())
          {
            ccp.Queue.rp = 0;
          }
          ccp.Queue.len--;
          CCP_ENABLE_INTERRUPT();
          return 1;
        }
        CCP_ENABLE_INTERRUPT();

        /* Send a pending DAQ message (DTM) */
      #else
        CCP_DISABLE_INTERRUPT();
        if (0 != (ccp.SendStatus&CCP_DTM_REQUEST))
        {
          ccp.SendStatus &= (CCP_BYTE)(~CCP_DTM_REQUEST); /* PRQA S 0277 */ /* MD_Ccp_0277 */
          CCP_ENABLE_INTERRUPT();
          Ccp_SendDtm();
          return 1;
        }
        CCP_ENABLE_INTERRUPT();
      #endif
    }
    #endif
  return 0;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*--------------------------------------------------------------------------*/
/* Initialization */
/*--------------------------------------------------------------------------*/
FUNC(void, CCP_CODE) Ccp_InitMemory( void )
{
  Ccp_InitializationState = CCP_UNINIT;

#if( CCP_CONTROL == STD_ON )
  Ccp_ControlState = kCcp_Control_Enable;
  Ccp_ControlDeniedCount = 0u;
#endif
}

/*******************************************************************************
** Function:    Ccp_Reset
**
** Purpose:     reset ccp status
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_Reset( void )
{
  /* Initialize all CCP variables to zero */
  CCP_BYTEPTR p;
  CCP_BYTEPTR pl;

#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  p = (CCP_BYTEPTR)&ccp; /* PRQA S 0310 */ /* MD_Ccp_0310 */
  pl = p+sizeof(ccp); /* PRQA S 0488 */ /* MD_Ccp_0488 */
  while (p<pl)
  {
    *p++ = 0; /* PRQA S 3440, 0489 */ /* MD_Ccp_3440 */ /* MD_Ccp_0489 */
  }

  Ccp_CanInit();
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_Init
**
** Purpose:     initialize the component
**
**  Parameter:  ConfigPtr : pointer to configuration structure in post build use case
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_Init( P2CONST(Ccp_ConfigType, AUTOMATIC, CCP_PBCFG) ConfigPtr )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  Ccp_InitializationState = CCP_UNINIT;

#if ( CCP_CHECK_INIT_POINTER == STD_ON )
  Ccp_CheckDetErrorReturnVoid( NULL_PTR != ConfigPtr,
                               CCP_INIT_SERVICE_ID, CCP_E_NULL_POINTER )
#endif

  Ccp_ConfigDataPtr = ConfigPtr;
  Ccp_Reset();

  Ccp_InitializationState = CCP_INIT;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


#if ( CCP_VERSION_INFO_API == STD_ON )
/**************************************************************************************************
   Function name    : CcpGetVersionInfo
   ------------------------------------------------------------------------------------------------
   Description      : Returns version information of module
   ------------------------------------------------------------------------------------------------
   Called by        : -
   ------------------------------------------------------------------------------------------------
   Parameter        : Pointer to location at which version information shall be stored at
   ------------------------------------------------------------------------------------------------
   Returncode       : void
   ------------------------------------------------------------------------------------------------
   Misc             : -
**************************************************************************************************/
FUNC(void, CCP_CODE) Ccp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CCP_APPL_DATA) CcpVerInfoPtr)
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  Ccp_CheckDetErrorReturnVoid( (P2VAR(Std_VersionInfoType, AUTOMATIC, CCP_APPL_DATA))NULL_PTR != CcpVerInfoPtr,
                                CCP_GETVERSIONINFO_SERVICE_ID, CCP_E_NULL_POINTER )

  /* Since this service only access non-volatile data and no channel parameter is passed,
     checking of the channel handle and initialization is omitted. */
  CcpVerInfoPtr->vendorID = (uint16)CCP_VENDOR_ID;
  CcpVerInfoPtr->moduleID = (uint16)CCP_MODULE_ID;
  CcpVerInfoPtr->sw_major_version = (uint8)(CP_ASR4CCP_VERSION >> 8u);
  CcpVerInfoPtr->sw_minor_version = (uint8)(CP_ASR4CCP_VERSION & 0xff);
  CcpVerInfoPtr->sw_patch_version = (uint8)CP_ASR4CCP_RELEASE_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* CCP_ENABLE_VERSION_INFO_API */



/*--------------------------------------------------------------------------*/
/* Test */
/* Screen output for test and diagnostics */
/*--------------------------------------------------------------------------*/

#endif

#define CCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* module specific MISRA deviations:
MD_Ccp_0277:      Rule 3.1
      Reason:     No side efects by this cast.
      Risk:       There is no risk as this has been used in the CANbedded path for a long time.
      Prevention: Covered by code review.
MD_Ccp_0306:      Rule 11.3
      Reason:     Cast to different object pointer type does not harm.
      Risk:       Pointer cast to uint32 is save on 32Bit platforms
      Prevention: Covered by code review.
MD_Ccp_0310:      Rule 11.4
      Reason:     Cast to different object pointer type does not harm.
      Risk:       Pointer casting is save as result is copied byte wise
      Prevention: Covered by code review.
MD_Ccp_0311:      Rule 11.5
      Reason:     Casting to remove const for optimization reason; target is in ROM, no change possible
      Risk:       Maintainability reduced due to optimization.
      Prevention: Covered by code review.
MD_Ccp_0488:      Rule 17.4
      Reason:     For optimization reasons, pointer arithmetic is used
      Risk:       No type cast is performed, therefore pointer arithmetic is save.
      Prevention: Covered by code review.
MD_Ccp_0489:      Rule 17.4
      Reason:     For optimization reasons, pointer arithmetic is used.
      Risk:       Pointer arithmetic is not type casted and therefore save
      Prevention: Covered by code review.
MD_Ccp_0770:      Rule 14.5
      Reason:     For optimization reasons more than one break or continue was used to leave the for loop.
      Risk:       Maintainability reduced due to multiple break or continue statements.
      Prevention: Covered by code review.
MD_Ccp_0841:      Rule 19.6
      Reason:     For historical reasons, this code is kept.
      Risk:       There is no risk as this has been used in the CANbedded path for a long time.
      Prevention: Covered by code review.
MD_Ccp_0842:      Rule 19.5
      Reason:     For historical reasons, this code is kept.
      Risk:       There is no risk as this has been used in the CANbedded path for a long time.
      Prevention: Covered by code review.
MD_Ccp_0842:      Rule 19.5
      Reason:     For historical reasons, this code is kept.
      Risk:       There is no risk as this has been used in the CANbedded path for a long time.
      Prevention: Covered by code review.
MD_Ccp_3305:
      Reason:     Pointer cast to stricter alignment is covered by 32bit union from Transport Layer.
      Risk:       There is no risk as this has been used in the CANbedded path for a long time.
      Prevention: Covered by code review.
MD_Ccp_3408:      Rule 8.8
      Reason:     The variable is not defined as static to be accessible for testing purposes.
      Risk:       There is no risk as this variable is not accessed externally otherwise.
      Prevention: Covered by code review.
MD_Ccp_3418:      Rule 12.10
      Reason:     The comma operator has been used to improve readability
      Risk:       There is no risk as the variables are only used in the for loop.
      Prevention: Covered by code review.
MD_Ccp_3440:      Rule 12.13
      Reason:     To be compatible with CANbeddedimplementations this is kept.
      Risk:       There is no risk as this has been used in the CANbedded path for a long time.
      Prevention: Covered by code review.
MD_Ccp_3673:      Rule 16.7
      Reason:     To be compatible with CANbeddedimplementations this is kept.
      Risk:       There is no risk as this has been used in the CANbedded path for a long time.
      Prevention: Covered by code review.
*/


