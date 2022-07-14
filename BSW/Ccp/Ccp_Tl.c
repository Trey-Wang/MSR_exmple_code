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
|    File Name:   Ccp_tl.c
|
|  Description: 
|   Implements the CCP Transport Layer module.
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
|                              - new function ccpGetDaqPointer
| 2001-30-05  1.32.00  Za      - Reserved word "data" in KEIL Compiler for C5x5
|                              - Prefix CCP_ for all #defines
| 2001-14-09  1.33.00  Za      - #define CCP_ODT_ENTRY_SIZE
|                              - #define CCP_INTEL,CCP_MOTOROLA
| 2001-28-10  1.34.00  Za      - ccpSend return value removed
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
#include "CanIf.h"                                   /* Include CAN Interface */

#if ( CCP_DEV_ERROR_DETECT == STD_ON )
#  include "Det.h"              /* Include Diagnostic Event Manager */
#endif

/******************************************************************************/
/* Defintions                                                                 */
/******************************************************************************/
#define kCcpMaxCTOMax 8
typedef union { /* PRQA S 0750 */ /* Union used to force proper alignment */
  /* There might be a loss of up to 3 bytes. */
  uint8  b[ ((kCcpMaxCTOMax + 3) & 0xFFC)      ];
  uint16 w[ ((kCcpMaxCTOMax + 3) & 0xFFC) / 2  ];
  uint32 dw[ ((kCcpMaxCTOMax + 3) & 0xFFC) / 4 ];
} tCcpCto;

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
  (void)Det_ReportError( CCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)); return (RET_VAL); } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define Ccp_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE ) { if(!(CONDITION)) { \
  (void)Det_ReportError( CCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)); } } /* PRQA S 3453, 3458 */ /* MD_MSR_19.7 */ /* MD_MSR_19.4 */
#define Ccp_CallDetReportError( API_ID, ERROR_CODE ) (void)Det_ReportError( CCP_MODULE_ID, 0, (API_ID),(ERROR_CODE)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define Ccp_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE )
# define Ccp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) 
# define Ccp_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE )
# define Ccp_CallDetReportError( API_ID, ERROR_CODE )
#endif

/* Initialization status of CCP. */
#if !defined ( CCP_UNINIT )
# define CCP_UNINIT   0
#endif
#if !defined ( CCP_INIT )
# define CCP_INIT     1
#endif

#define CCP_TX_PDU_STATE_IDLE 0
#define CCP_TX_PDU_STATE_PENDINGMAIN 1
#define CCP_TX_PDU_STATE_PENDINGTXCONF 2
#define CCP_TX_PDU_STATE_LOCKED 3


/*******************************************************************************
 * Data definitions
*******************************************************************************/
/* global variables */
#define CCP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(PduIdType, CCP_VAR_NOINIT) Ccp_RxPduId;
STATIC volatile VAR(uint8, CCP_VAR_NOINIT) Ccp_TxPduState;

#define CCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define CCP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC volatile VAR(uint8, CCP_VAR_ZERO_INIT) CcpCan_InitializationState = CCP_UNINIT;

#define CCP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define CCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*******************************************************************************
** Function:    Ccp_RxIndication
**
** Purpose:     Rx Indication for reception of CTO and DTO Packets
**              This function is configured in the generation tool.
**
**  Parameter:  
**              CanCanCcpRxPduId : Target PDU ID of CAN L-PDU that has been received.
**              CanSduPtr        : Pointer to received L-SDU (payload).
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_RxIndication
(
  PduIdType CanCanCcpRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, CCP_APPL_VAR) PduInfoPtr
)
{
  /* An aligned field of 8 Byte, which is passed to ccpCommand.       */
  /* Deviation of MISRA rule 110 (an aligned byte array is required). */
  tCcpCto c; /* PRQA S 0759 */ /* Union used to force proper alignment */

#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  Ccp_CheckDetErrorReturnVoid( CCP_UNINIT != CcpCan_InitializationState,
                               CCP_RXINDICATION_SERVICE_ID, CCP_E_NOT_INITIALIZED )
  Ccp_CheckDetErrorReturnVoid( PduInfoPtr != NULL_PTR,
                               CCP_RXINDICATION_SERVICE_ID, CCP_E_NULL_POINTER )
  Ccp_CheckDetErrorReturnVoid( PduInfoPtr->SduDataPtr != NULL_PTR,
                               CCP_RXINDICATION_SERVICE_ID, CCP_E_NULL_POINTER )
  Ccp_CheckDetErrorReturnVoid( CanCanCcpRxPduId < Ccp_GetSizeOfChannel(),
                               CCP_RXINDICATION_SERVICE_ID, CCP_E_INVALID_PDUID )
#if (CCP_INVALIDHNDOFCHANNEL == STD_ON) 
  Ccp_CheckDetErrorReturnVoid( !Ccp_IsInvalidHndOfChannel(CanCanCcpRxPduId),
                               CCP_RXINDICATION_SERVICE_ID, CCP_E_INVALID_PDUID )
#endif

  /* Store the Rx PDU Id for the reception. */
  Ccp_RxPduId = CanCanCcpRxPduId;

  /*
    A CCP Packet has been received.
    Copy the whole received SDU (8 Byte) to an 32-Bit aligned buffer.
  */
  c.b[0] = PduInfoPtr->SduDataPtr[0];
  c.b[1] = PduInfoPtr->SduDataPtr[1];
  c.b[2] = PduInfoPtr->SduDataPtr[2];
  c.b[3] = PduInfoPtr->SduDataPtr[3];
  c.b[4] = PduInfoPtr->SduDataPtr[4];
  c.b[5] = PduInfoPtr->SduDataPtr[5];
  c.b[6] = PduInfoPtr->SduDataPtr[6];
  c.b[7] = PduInfoPtr->SduDataPtr[7];

  Ccp_Command((CCP_BYTEPTR)&c.dw[0]); /* PRQA S 0310 */ /* Target Pointer castet to 32bit aligned frame buffer */
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_TxConfirmation
**
** Purpose:     Tx Confirmation for successful transmission of CTO and DTO Packets
**              This function is configured in the generation tool.
**
**  Parameter:  
**              CanCanCcpTxPduId : PDU ID of CAN L-PDU transmitted successfully.
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_TxConfirmation( PduIdType CanTxPduId )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  Ccp_CheckDetErrorReturnVoid( CCP_UNINIT != CcpCan_InitializationState,
                               CCP_TXCONFIRMATION_SERVICE_ID, CCP_E_NOT_INITIALIZED )

  CCP_DUMMY_STATEMENT( CanTxPduId ); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  if(Ccp_TxPduState == CCP_TX_PDU_STATE_LOCKED)
  {
    Ccp_TxPduState = CCP_TX_PDU_STATE_PENDINGMAIN;
  }
  else
  {
    Ccp_TxPduState = CCP_TX_PDU_STATE_IDLE;
    (void)Ccp_SendCallBack();
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_Send
**
** Purpose:     Request for the transmission of a DTO or CTO message
**
**  Parameter:  
**              len : length of PDU data
**              msg : pointer to PDU data
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_Send( CCP_BYTEPTR msg )
{
  PduInfoType PduInfo;

#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  Ccp_CheckDetErrorReturnVoid( CCP_UNINIT != CcpCan_InitializationState,
                               CCP_SEND_SERVICE_ID, CCP_E_NOT_INITIALIZED )

  PduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, CCP_APPL_VAR))msg;
  PduInfo.SduLength  = (PduLengthType)8u;

  Ccp_TxPduState = CCP_TX_PDU_STATE_LOCKED;
  if ( CanIf_Transmit( Ccp_GetCanIfTxPduIdOfChannel(Ccp_RxPduId), &PduInfo ) != (Std_ReturnType)E_OK )
  {
      Ccp_TxPduState = CCP_TX_PDU_STATE_IDLE;
      /* Fatal Error. CCP Packet cannot be transmitted. */
      Ccp_Reset();
  }

  CCP_DISABLE_INTERRUPT();
  if(Ccp_TxPduState == CCP_TX_PDU_STATE_LOCKED)
  {
    Ccp_TxPduState = CCP_TX_PDU_STATE_PENDINGTXCONF;
  }
  CCP_ENABLE_INTERRUPT();
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/*******************************************************************************
** Function:    Ccp_UserMainFunction
**
** Purpose:     Cyclic MainFunction handling
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_UserMainFunction( void )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  CCP_DISABLE_INTERRUPT();
  if(Ccp_TxPduState == CCP_TX_PDU_STATE_PENDINGMAIN)
  {
    Ccp_TxPduState = CCP_TX_PDU_STATE_IDLE;
    CCP_ENABLE_INTERRUPT();
    Ccp_TxConfirmation(Ccp_RxPduId);
  }
  else
  {
    CCP_ENABLE_INTERRUPT();
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*******************************************************************************
** Function:    Ccp_CanInitMemory
**
** Purpose:     Initializations of the CCP Transport Layer.
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_CanInitMemory( void )
{
  CcpCan_InitializationState = CCP_UNINIT; /* PRQA S 3198 */ /* Variable is set in case any other service is called in interrupt context */
}


/*******************************************************************************
** Function:    Ccp_CanInit
**
** Purpose:     Initializations of the CCP Transport Layer.
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CCP_CODE) Ccp_CanInit( void )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return;
  }
#endif

  CcpCan_InitializationState = CCP_UNINIT; /* PRQA S 3198 */ /* Variable is set in case any other service is called in interrupt context */

  Ccp_RxPduId = 0;
  Ccp_TxPduState = CCP_TX_PDU_STATE_IDLE;

  CcpCan_InitializationState = CCP_INIT;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*******************************************************************************
** Function:    Ccp_GetCurrentCANId
**
** Purpose:     Returns the current active Tx CAN Id
**
**  Parameter:  -
**
**  Returncode: -
*******************************************************************************/
FUNC(uint32, CCP_CODE) Ccp_CanGetCurrentCANId( void )
{
#if( CCP_CONTROL == STD_ON )
  if( Ccp_ControlState != kCcp_Control_Enable )
  {
    Ccp_ControlDeniedCount++;
    return 0;
  }
#endif

  Ccp_CheckDetErrorReturnValue( CCP_UNINIT != CcpCan_InitializationState,
                                CCP_CANGETCURRENTCANID_SERVICE_ID, CCP_E_NOT_INITIALIZED, 0u )

  return( Ccp_GetCanTxMsgIdOfChannel(Ccp_RxPduId) );
}/* PRQA S 2006 */ /* MD_MSR_14.7 */


#define CCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

