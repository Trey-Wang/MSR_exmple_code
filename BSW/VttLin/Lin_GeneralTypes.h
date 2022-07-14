/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *           File:  Lin_GeneralTypes.h
 *      Component:  AUTOSAR LIN Driver
 *
 *    Description:  Type definitions of the AUTOSAR LIN Driver
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Lutz Pflueger                 vislpr        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  05.00.00  2013-08-12  vislpr  -             Port Canoeemu to VIP
 *  05.01.00  2014-07-17  vislpr  -             Atomic access to VipCntrl, bugfixing
 *  06.00.00  2014-09-29  vislpr  -             Add Canoeemu to VIP, ComStackLib, R11, PB-S
 *  06.00.01  2014-11-20  vislpr  -             HL Part was updated
 *  07.00.00  2015-01-29  vislpr  ESCAN00080255 FEAT-254: Globale Umbennenung von VIP nach VTT
 *                        vislpr  ESCAN00081345 FEAT-427: SafeBSW Step I
 *  07.01.00  2015-06-15  vislpr  ESCAN00083109 VTT Lin shall derive its interrupt offset from the values configured in the OS
 *  07.02.00  2015-06-15  vislpr  -             Remove EMU and HL part dependencies 
 *  07.03.00  2016-04-15  vislpr  -             HL Part was updated
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Friedrich Kiesel              visfki        Vector Informatik GmbH
 *  Lutz Pflueger                 vislpr        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY - CORE -
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2012-07-20  visfki  ESCAN00059300 Implementation of High Level AUTOSAR 4 LIN Driver
 *  04.01.00  2012-11-20  visfki  ESCAN00061782 AR4-220: Remove STATIC
 *                                ESCAN00063147 Rename Lin_WakeUp to Lin_Wakeup
 *  04.02.00  2013-02-08  visfki  ESCAN00064956 AR4-325: Implement Post-Build Loadable
 *                                ESCAN00065149 Compiler error: LIN_CH_OPERATIONAL changed in ASR4 to LIN_OPERATIONAL
 *  05.00.00  2013-09-19  vislpr  ESCAN00066851 Improve include structure for EcuM headers
 *                                ESCAN00079533 AR4-260: Remove ProdErrorDetection configuration switch
 *                                ESCAN00069952 Justify MISRA Warnings on functions
 *                                ESCAN00067708 AR4-328: Predefined Runtime Measurement Points
 *  05.00.01  2013-11-19  vislpr  ESCAN00071647 Remove MISRA warning 0850/3453
 *                                ESCAN00072030 Change RTM macros
 *  05.00.02  2013-11-21  vislpr  ESCAN00072030 Fix MISRA Warnings and GNU Compiler error on RTM
 *  06.00.00  2014-10-01  vislpr  ESCAN00076482 AR4-832: External wake up via LIN transceiver
 *                                ESCAN00078696 AR4-698: Post-Build Selectable (Identity Manager)
 *  06.00.01  2014-11-14  vislpr  ESCAN00079471 Compiler error: lin.c: 'Lin_Dev_InitDetect'/'LIN_DEV_INITVALUE'
 *  06.00.02  2014-11-14  vislpr  ESCAN00079588 Compiler error: Identifier Channel not declared
 *  06.00.03  2014-12-02  vislpr  -             Change wakeup handling
 *  07.00.00  2014-12-12  vislpr  ESCAN00080307 FEAT-427: SafeBSW Step I
 *  07.00.01  2015-01-27  vislpr  ESCAN00080823 Add channel DET check for out of bounds of channel id and channel idx
 *                        vislpr  ESCAN00080824 Use LOCAL_INLINE macro of Compiler.h instead "static inline"
 *  07.00.02  2015-03-26  vislpr  ESCAN00082080 Remove redundant code in Lin_GoToSleep
 *  07.00.03  2015-05-26  vislpr  ESCAN00083154 Dem.h not included if Dem repoting enabled
 *                        vislpr  ESCAN00083156 EcuM_BswErrorHook() are used on precompile with Selectable enabled
 *  07.01.00  2015-07-13  vislpr  ESCAN00081794 FEAT-1275: SafeBSW Step 2
 *  07.01.01  2015-09-15  vislpr  ESCAN00085267 Unkown return value 0x01u of Lin_GetStatus() in case of LIN_TX_BUSY error
 *  07.01.02  2015-10-23  vislpr  ESCAN00086056 Check data length parameter of Lin_SendFrame()
 *  07.02.00  2015-11-04  vislpr  ESCAN00086254 Unification the register base address struct and typedef for each driver
 *            2015-11-04  vislpr  ESCAN00086267 Code refactoring
 *            2015-11-16  vislpr  ESCAN00086493 Remove Lin_GetRxPin() Macro.
 *            2015-11-16  vislpr  ESCAN00086494 Do not set the state to wake after calling Lin_CheckWakeup()
 *            2015-11-17  vislpr  ESCAN00086546 Inconsistency of wakeup support between Lin, LinIf and EcuM in variant Post Build
 *  07.03.00  2016-01-27  vislpr  ESCAN00087812 FEAT-1688: SafeBSW Step 4
 *  07.03.01  2016-03-16  vislpr  -             SafeBSW review rework
 *  07.04.00  2016-04-13  vislpr  -             SafeBSW review rework (ISR)
 *********************************************************************************************************************/
 
#if !defined (LIN_GENERALTYPES_H)
#define LIN_GENERALTYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* Include General Types of DrvLin:
    The "Lin_DrvGeneralTypes.h" header file is generated and defines the LIN_TRCV_ENABLED switch 
    if the transceiver in LinIf module is enabled ([ANY]/LinIf/LinIfGeneral/LinIfTrcvDriverSupported). */
#include "Lin_DrvGeneralTypes.h"

/*! LIN operation states for a LIN channel or frame, as returned by the API service Lin_GetStatus(). */
typedef enum Lin_StatusTypeTag
{
  /*! LIN frame operation return value.
      Development or production error occurred. */
  LIN_NOT_OK = 0x0BU,
  
  /*! LIN frame operation return value.
      Successful transmission.    */
  LIN_TX_OK = 0x00U,  

  /*! LIN frame operation return value.
      Ongoing transmission (Header or Response). */
  LIN_TX_BUSY = 0x01U,

  /*! LIN frame operation return value.
      Erroneous header transmission such as: 
       - Mismatch between sent and read back data 
       - Identifier parity error or 
       - Physical bus error */
  LIN_TX_HEADER_ERROR = 0x02U,
  
  /*! LIN frame operation return value.
      Erroneous response transmission such as: 
       - Mismatch between sent and read back data 
       - Physical bus error */
  LIN_TX_ERROR = 0x03U,
 
  /*! LIN frame operation return value.
      Ongoing reception: at least one response byte has been received, but the checksum byte has not been received. */
  LIN_RX_BUSY = 0x05U,

  /*! LIN frame operation return value.
      Reception of correct response. */
  LIN_RX_OK = 0x04U,
  
  /*! LIN frame operation return value. 
      Erroneous response reception such as: 
       - Framing error 
       - Overrun error 
       - Checksum error or 
       - Short response */
  LIN_RX_ERROR = 0x06U,
 
  /*! LIN frame operation return value.  
      No response byte has been received so far. */
  LIN_RX_NO_RESPONSE = 0x07U,
  
  /*! LIN channel state return value.  
      Normal operation; the related LIN channel is ready to transmit next header. No data from previous frame available
      (e.g. after initialization). */
  LIN_OPERATIONAL = 0x09U,
 
  /*! LIN channel state return value.  
      Sleep state operation; in this state wake-up detection from slave nodes is enabled. */
  LIN_CH_SLEEP = 0x0AU
}Lin_StatusType;


/*! Pointer to pointer to a shadow buffer or memory mapped LIN Hardware receive buffer where the current SDU is
    stored. */ 
typedef P2VAR(uint8, TYPEDEF, LIN_APPL_VAR) Lin_u8PtrType;


/*! Represents all valid protected identifier used by Lin_SendFrame(). */
typedef uint8 Lin_FramePidType;


/*! This type is used to specify the Checksum model to be used for the LIN Frame. */
typedef enum Lin_FrameCsModelTypeTag
{
  LIN_ENHANCED_CS = 0, /*<! Enhanced checksum model */
  LIN_CLASSIC_CS  = 1  /*<! Classic checksum model */
}Lin_FrameCsModelType;


/*! This type is used to specify whether the frame processor is required to transmit the 
    response part of the LIN frame. */
typedef enum Lin_FrameResponseTypeTag
{
  LIN_MASTER_RESPONSE = 0, /*<! Response is generated from this (master) node */
  LIN_SLAVE_RESPONSE  = 1, /*<! Response is generated from a remote slave node */
  LIN_SLAVE_TO_SLAVE  = 2  /*<! Response is generated from one slave to another slave */
}Lin_FrameResponseType;


/*! This type is used to specify the number of SDU data bytes to copy.
    Range: 1 - 8, data length of a LIN frame. */
typedef uint8 Lin_FrameDlType;


/*! This Type is used to provide PID, checksum model, data length and SDU pointer of a LIN frame
    from the LIN Interface to the LIN driver.  */
typedef struct Lin_PduTypeTag
{
  VAR(Lin_FramePidType, TYPEDEF)      Pid;    /*<! Valid protected identifier */
  VAR(Lin_FrameCsModelType, TYPEDEF)  Cs;     /*<! Specified Checksum model */
  VAR(Lin_FrameResponseType, TYPEDEF) Drc;    /*<! Type of response part */
  VAR(Lin_FrameDlType, TYPEDEF)       Dl;     /*<! SDU data bytes to copy */
  P2VAR(uint8, TYPEDEF, LIN_APPL_VAR) SduPtr; /*<! Pointer to SDU data bytes */
}Lin_PduType;


/* Include General Types of LinIf */
#include "LinIf_GeneralTypes.h"

/* Optional include General Types of LinTrcv */
#if defined LIN_TRCV_ENABLED                                                                                            /* COV_LIN_INTEGRATION */
# include "LinTrcv_GeneralTypes.h"
#endif

#endif /* LIN_GENERALTYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Lin_GeneralTypes.h
 *********************************************************************************************************************/
