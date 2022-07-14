/*****************************************************************************
| Project Name:   C C P - Driver
|    File Name:   Ccp_Appl.c
|
|  Description: 
|   Implements the CCP module application call-backs.
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
| 2017-01-10  3.00.00  Hr      Rework of component based on Cp_AsrCcp
|                      Hr      ESCAN00089034: Frame end check during DAQ assembly not correct
|***************************************************************************/

/* CCP Definitions and Parameters */
#include "Ccp.h"


#define CCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */


#if ( CCP_MEM_PROTECTION == STD_ON )

/*****************************************************************************
| NAME:             Ccp_CheckReadAccess
| CALLED BY:        Ccp_Command
| PRECONDITIONS:    
| INPUT PARAMETERS: a : address of parameter to read
|                   size : size of parameter to read
| RETURN VALUES:    CCP_READ_DENIED or CCP_READ_OK
| DESCRIPTION:      
******************************************************************************/
FUNC(CCP_BYTE, CCP_CODE) Ccp_CheckReadAccess( CCP_MTABYTEPTR a, CCP_BYTE size )
{
  return CCP_READ_OK;
}

/*****************************************************************************
| NAME:             Ccp_CheckWriteAccess
| CALLED BY:        Ccp_Command
| PRECONDITIONS:    
| INPUT PARAMETERS: a : address of parameter to write
|                   size : size of parameter to write
| RETURN VALUES:    CCP_WRITE_DENIED or CCP_WRITE_OK
| DESCRIPTION:      
******************************************************************************/
FUNC(CCP_BYTE, CCP_CODE) Ccp_CheckWriteAccess( CCP_MTABYTEPTR a, CCP_BYTE size )
{
  return CCP_WRITE_OK;
}
#endif

/*****************************************************************************
| NAME:             Ccp_GetPointer
| CALLED BY:        Ccp_Command
| PRECONDITIONS:    
| INPUT PARAMETERS: addr_ext : address extension of memory location
|                   addr : address of memory location
| RETURN VALUES:    pointer to memory location
| DESCRIPTION:      
******************************************************************************/
FUNC(CCP_MTABYTEPTR, CCP_CODE) Ccp_GetPointer( CCP_BYTE addr_ext, CCP_DWORD addr )
{
  return( (CCP_MTABYTEPTR)addr );
}

#define CCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */  /*  MD_MSR_19.1 */

