/* Kernbauer Version: 1.16 Konfiguration: DrvTransCan Erzeugungsgangnummer: 340 */

/**********************************************************************************************************************
 |               C O P Y R I G H T
 |-----------------------------------------------------------------------
 |------  Copyright (c) 2000-2016 by Vector Informatik GmbH, all rights 
 |reserved
 |
 | Please note, that this file contains an implementation example for 
 | CanTrcv. This code may influence the behaviour of the CanTrcv
 | in principle. Therefore, great care must be taken to verify
 | the correctness of the implementation.
 |
 | The contents of the originally delivered files are only examples resp. 
 | implementation proposals. With regard to the fact that these functions 
 | are meant for demonstration purposes only, Vector Informatik's 
 | liability shall be expressly excluded in cases of ordinary negligence, 
 | to the extent admissible by law or statute.
 |  -------------------------------------------------------------------------------------------------------------------
 |  FILE DESCRIPTION
 |  -------------------------------------------------------------------------------------------------------------------
 |         File:  CanTrcv_30_GenericCan.c
 |      Project:  MICROSAR
 |       Module:  CAN Transceiver driver
 |    Generator:  DaVinci Configurator 5
 |  Description:  Implementation of the MICROSAR CAN Transceiver Driver.
 **********************************************************************************************************************/

 /**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Senol Cendere                 Ces           Vector Informatik GmbH
 *  Matthias Fleischmann          Fn            Vector Informatik GmbH
 *  Mihai Olariu                  Ou            Vector Informatik GmbH
 *  Timo Vanoni                   Tvi           Vector Informatik GmbH
 *  Eugen Stripling               Seu           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00   2007-06-12  Ces     -             First Implementation
 *  1.01.00   2008-07-11  Ou      -             Not usable version because the generic transceiver driver was addapted with the hand (according to the process HL + Ll_template)
 *  1.02.00   2008-07-14  Ou      -             Separate the template version of generic transceiver driver low-level version
 *  1.03.00   2008-07-25  Ou      ESCAN00028568 Compile error: Versions of generated files does not map to static files
 *  1.04.00   2008-08-21  Ou      ESCAN00029373 Version scanning fails for the generic transceiver driver
 *  1.05.00   2008-09-12  Ou      -             Create intermediate macros for low-level version
 *  1.06.00   2008-10-08  Ou      -             Remove the V_SUPPRESS_EXTENDED_VERSION_CHECK encapsulations, the DLLs versions are always checked
 *  1.07.00   2008-10-28  Ou      -             No changes, just update the specific DLLs
 *  1.08.00   2008-12-11  Ou      -             Use the Autosar compliant copyright
 *  1.09.00   2009-02-19  Tvi     -             Changed to match with updated DLLs
 *  1.10.00   2009-02-26  Tvi     ESCAN00033398 Compiler warnings when using different transceiver types within the same stack
 *  1.11.00   2009-07-01  Fn      -             (no changes here, just to merge with HL)
 *            2009-07-27  Tvi     ESCAN00033879 Define is written wrong and not enclosured with a switch
 *  1.11.01   2009-10-16  Tvi     -             (no changes here, just to merge with HL)
 *  1.12.00   2009-11-04  Tvi     -             (no changes here, just to merge with HL)
 *  1.13.00   2010-05-04  Tvi     -             (no changes here, just to merge with HL)
 *  1.14.00   2010-12-06  Tvi     ESCAN00047454 Improve comments for Generic Trcv
 *  1.14.01   2011-02-08  Tvi     -             (no changes here, just to merge with HL)
 *  1.14.02   2011-04-04  Tvi     -             (no changes here, just to merge with HL)
 *  2.00.00   2011-07-12  Tvi     -             (no changes here, just to merge with HL)
 *  2.01.00   2011-11-28  Tvi     ESCAN00053775 provide Generic Transceiver driver for partial networking
 *  2.01.01   2012-05-03  Tvi     -             (no changes here, just to merge with HL)
 *  3.00.00   2012-07-30  Tvi     ESCAN00060109 AR3-2406: Asynchronous transceiver handling according to AUTOASR 3.2.2
 *  3.00.01   2012-09-27  Tvi     -             (remerge with CFG5 support)
 *  3.01.00   2013-04-04  Tvi     -             Add new macro "WupIsValid"
 *  3.02.00   2014-02-05  Tvi     -             (no changes here, just to merge with HL)
 *  4.00.00   2014-10-15  Seu     ESCAN00078952 AR4-698: Post-Build Selectable (Identity Manager)
 *  4.00.01   2015-07-29  Seu     -             (no changes here, just to merge with HL)
 *  4.01.00   2015-10-21  Seu     -             Merge with new HL
 *            2015-10-22  Seu     ESCAN00086019 Harmonise the default VendorApiInfix to "GenericCan"
 *  4.01.01   2016-10-17  Seu     -             Merge with new HL
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * ATTENTION: Replace the placeholders GenericCan and GENERICCAN with the according name of the used transceiver.
 *            GENERICCAN is used for definitions in upper case.
 *            GenericCan is used for variables in camel case.
 * EXAMPLE:   Replace GENERICCAN to TJA1041 and GenericCan to Tja1041.
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Matthias Fleischmann          Fn            Vector Informatik GmbH
 *  Mihai Olariu                  Ou            Vector Informatik GmbH
 *  Timo Vanoni                   Tvi           Vector Informatik GmbH
 *  Eugen Stripling               Seu           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2008-02-25  Fn      -             Intitial revision of the core implementation
 *  01.00.01  2008-04-17  Fn      -             Update of the include file names
 *  01.01.00  2008-06-26  Ou      ESCAN00027616 Various compile errors, problems with Organi
 *  01.02.00  2008-09-12  Ou      ESCAN00029980 Compiler failure due to wrong definition of SW version
 *  01.03.00  2008-10-07  Ou      ESCAN00028298 Several warnings due to unreferenced local variables
 *                        Ou      ESCAN00029572 DrvTrans__coreCanxAsr raise DET error if CanTrcv_SetOpMode is called with invalid TrcvWakeupMode
 *                        Ou      ESCAN00030536 Compile error at extended version check
 *  01.04.00  2008-12-11  Ou      ESCAN00031771 Adjust wakeup by bus handling for platforms which cannot be woken up with a CAN frame
 *  01.05.00  2009-02-19  Tvi     ESCAN00033258 GetBusWuReason may return an invalid value
 *  01.06.00  2009-02-26  Tvi     ESCAN00033383 Don't throw DET error in case the same mode is requested again
 *                        Tvi     ESCAN00033398 Compiler warnings when using different transceiver types within the same stack
 *                        Tvi     ESCAN00033413 Possible out of bounds access in SetOpMode if DET is enabled
 *  02.00.00  2009-06-29  Fn      ESCAN00035913 CAN Transceiver channel does not wake up
 *  02.00.01  2009-10-15  Tvi     ESCAN00038495 Compile error: unknown type definitions in dio.h
 *  02.01.00  2009-11-04  Tvi     ESCAN00036841 OP_MODE_SLEEP should be independent of GENERAL_WAKE_UP_SUPPORT
 *                        Tvi     ESCAN00038945 Use different DioPins tokens for GetOpMode and CHECK_WA
 *                        Tvi     ESCAN00038946 Rename filenames to match BSW00347
 *                        Tvi     ESCAN00038947 CanTrcv087 has to be implemented the correct way
 *                        Tvi     ESCAN00039039 Add support for timers
 *                        Tvi     ESCAN00039041 Allow transition to OP_MODE_NORMAL even if it is the current OpMode
 *                        Tvi     ESCAN00039043 Implement an initialization via OP_MODE_NORMAL
 *                        Tvi     ESCAN00039194 Disable IcuNotification when entering Normal mode in Init and SetOpMode
 *  02.02.00  2010-05-04  Tvi     ESCAN00040088 Compiler Warning: unused parameter 'TrcvWakeupMode'
 *  02.20.00  2010-07-01  Tvi     ESCAN00046512 Add support for partial network transceivers
 *            2010-11-29  Tvi     ESCAN00046781 Support Multiple Identity Configuration
 *                                ESCAN00045601 "Channel Used" parameter not well implemented
 *                                ESCAN00046813 Adapt AUTOSAR version check
 *                                ESCAN00047668 Critical areas should be Trcv specific
 *  02.20.01  2011-02-08  Tvi     ESCAN00048498 Wrong DEM Events used
 *  02.20.02  2011-03-11  Tvi     ESCAN00049830 Remove ASR Version check for SPI / DIO
 *                                ESCAN00049831 [PN only] Store wakeup event and report them if Trcv is in Standby mode
 *  03.00.00  2011-05-05  Tvi     ESCAN00051930 Add support for ASR3.2.1
 *                                ESCAN00051564 Remove the AUTOSAR Release Version Check
 *  03.01.00  2011-11-22  Tvi     ESCAN00054349 AR3-1738: Add support for asynchronous transceiver driver
 *                                ESCAN00053272 AR3-1698: Add MISRA justifications in source code for automatic generation of MISRA reports
 *                                ESCAN00053265 AR3-2069: Remove non-SchM code for critical section handling
 *  03.01.01  2012-02-12  Tvi     ESCAN00057892 [PN only] CanTrcv does not wakeup when Synchronous SPI is being used
 *                        Tvi     ESCAN00056788 Compiler error/warning: DRVTRANS__CORECANXASR_RELEASE_VERSION' redefined
 *  04.00.00  2012-05-21  Tvi     ESCAN00058641 AR4-95: Provide CanTrcv according ASR 4, Asynchronous Transceiver Mode Changes
 *                                ESCAN00059656 Add support for SetPNActivationState according to ASR4
 *                                ESCAN00060455 AR3-2406: Asynchronous transceiver handling according to AUTOASR 3.2.2
 *  04.00.01  2012-09-27  Tvi     ESCAN00061779 [CFG5/ASR4 only]: Compiler error about unkown struct member instanceID
 *  04.01.00  2012-10-08  Tvi     ESCAN00062121 Report DEM if PN is not available
 *                        Tvi     ESCAN00066283 AR4-352: Support partial networking
 *  04.02.00  2013-11-20  Tvi     ESCAN00071370 AR4-569: [CanTrcv]  Incompatible  argument  in call to EcuM_SetWakeupEvent()
 *  04.03.00  2014-01-10  Tvi     ESCAN00073861 AR4-345: Implement PF4_PostBuildSelectable
 *  05.00.00  2014-09-24  Seu     ESCAN00076622 AR4-698: Post-Build Selectable (Identity Manager)
 *  05.00.01  2014-11-13  Seu     -             MISRA deviation comment for rule 6080 added (APIs: *_ClearTrcvWufFlag() and *_CheckWakeFlag())
 *  05.01.00  2015-05-04  Seu     ESCAN00077814 Support for CAN Transceiver on System-Basis Chips (SBCs)
 *            2015-07-23  Seu     ESCAN00084154 Only since R13: Compiler error: C2059: syntax error : '{'
 *            2015-07-23  Seu     ESCAN00083583 Adapt in order to fix ESCAN00083361 / ESCAN00083362
 *            2015-07-23  Seu     ESCAN00083395 Compiler warning: parameter is never used
 *  05.01.01  2015-09-24  Seu     ESCAN00085451 Support usage of Partial Networking for SBCs.
 *  05.01.02  2015-10-06  Seu     ESCAN00085678 Initialize requestState in case of INTERFACE_SBC and PN is used
 *  05.02.00  2015-02-04  Seu     ESCAN00080310 FEAT-427: SafeBSW Step I
 *            2015-10-05  Seu     ESCAN00081795 FEAT-1275: SafeBSW Step 2
 *            2015-10-05  Seu     ESCAN00085154 FEAT-1527: SafeBSW Step 3
 *  05.02.01  2016-10-17  Seu     ESCAN00091616 Compiler error: identifier index not declared
 * *******************************************************************************************************************/

/* PRQA S 0715 EOF */ /* MD_CanTrcv_30_GenericCan_0715 */
/* PRQA S 0850 EOF */ /* MD_MSR_19.8 */
/* PRQA S 0777 EOF */ /* MD_CanTrcv_30_GenericCan_0777 */
/* PRQA S 3453 EOF */ /* MD_MSR_19.7 */
/* PRQA S 0779 EOF */ /* MD_CanTrcv_30_GenericCan_0779 */

/* ********************************************************************************************************************
 *  INCLUDES
 * *******************************************************************************************************************/
#define CANTRCV_30_GENERICCAN_INCLUDE_INTERNAL
#define V_CANTRCV_30_GENERICCAN_SOURCE

#include "CanTrcv_30_GenericCan.h" /* SREQ-10148,SREQ-7523 */

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
# include "Dio.h" /* SREQ-10148,SREQ-7523 */
#endif

#include "CanIf.h" /* SREQ-10148,SREQ-7523 */

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
# include "Det.h" /* SREQ-10148,SREQ-7523 */
#endif

#include "EcuM.h" /* SREQ-10148,SREQ-7523 */

/* *********************************************************************************************************************
 *  VERSION CHECK
 * ********************************************************************************************************************/

#if (DRVTRANS__CORECANXASR_VERSION                             != 0x0502u)
# error "Module CanTrcv_30_GenericCan: Source and Header files are inconsistent!"
#endif
#if (DRVTRANS__CORECANXASR_RELEASE_VERSION                     != 0x01u)
# error "Module CanTrcv_30_GenericCan: Source and Header files are inconsistent!"
#endif

/* ** Version Check: CFG5 - Base part ** */
#if (CANTRCV_30_GENERICCAN_GENTOOL_CFG5_BASE_COMP_VERSION != 0x0102u)
# error "Module CanTrcv_30_GenericCan: Source and Generated Header files are inconsistent!"
#endif

#if (DRVTRANS_GENERICCANDIOASR_VERSION                        != 0x0401u)
# error "Module CanTrcv_30_GenericCan: Source and Header files are inconsistent!"
#endif
#if (DRVTRANS_GENERICCANDIOASR_RELEASE_VERSION                != 0x01u)
# error "Module CanTrcv_30_GenericCan: Source and Header files are inconsistent!"
#endif

/* ** Version Check: CFG5 - HW specific part ** */
#if (CANTRCV_30_GENERICCAN_GENTOOL_CFG5_HW_COMP_VERSION != 0x0102u)
# error "Module CanTrcv_30_GenericCan: Source and Generated Header files are inconsistent!"
#endif

#if defined(CANTRCV_30_GENERICCAN_USE_EXTENDED_WU_SRC)
#else
# define CANTRCV_30_GENERICCAN_USE_EXTENDED_WU_SRC STD_OFF
#endif

# if defined(CANTRCV_30_GENERICCAN_HW_PN_SUPPORT) && (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
/* It is not possible to enable partial networking with this driver. Within all descriptions, 
   the information about partial networking can be ignored. */
#  error "partial networking is not supported with this driver implementation!"
# endif

/* ********************************************************************************************************************
 *  LOCAL DEFINES
 * ********************************************************************************************************************/
#if defined(STATIC)
#else
# define STATIC static
#endif

#if !defined (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT)
# define CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT  CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT 
#endif

#if !defined (CANTRCV_30_GENERICCAN_DUMMY_STATEMENT)
# define CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(var)  (var) = (var) 
#endif

/* CREQ-102496 */
#define CanTrcvCfg_IsChannelUsed(ch)              CanTrcv_30_GenericCan_IsChannelUsed(ch)
#define CanTrcvCfg_IsWakeupByBusUsed(ch)          CanTrcv_30_GenericCan_IsWakeupByBusUsed(ch)
#define CanTrcvCfg_GetWakeupSource(ch)            CanTrcv_30_GenericCan_GetWakeupSource(ch)
#define CanTrcvCfg_GetWakeupSourceSyserr(ch)      CanTrcv_30_GenericCan_GetWuSrcSyserr(ch)
#define CanTrcvCfg_GetWakeupSourcePor(ch)         CanTrcv_30_GenericCan_GetWuSrcPor(ch)
#define CanTrcvCfg_IsIcuChannelSet(ch)            CanTrcv_30_GenericCan_IsIcuChannelSetOfChannel(ch)
#define CanTrcvCfg_GetIcuChannel(ch)              CanTrcv_30_GenericCan_GetIcuChannelOfChannel(ch)

/*  SupportedModes
    --------------
    
    Specify the modes and features supported by the underlying transceiver
    hardware by enabling the following defines.

    Enable this if the underlying transceiver hardware supports a standby mode
    
    #define CANTRCV_30_GENERICCAN_SUPPORTS_MODE_STANDBY

    Enable this if the underlying transceiver Hardware supports a sleep mode
    
    #define CANTRCV_30_GENERICCAN_SUPPORTS_MODE_SLEEP

    Enable this to init via normal mode. This is needed by the Tja1041 to clear 
    the pwon flag and allow the transceiver to enter sleep from init
    
    #define CANTRCV_30_GENERICCAN_INIT_VIA_NORMAL_MODE

    Enable this to allow the transceiver switching to normal mode again. This 
    is needed by the Tja1041 to toggle the WAKE pin.
    
    #define CANTRCV_30_GENERICCAN_ALLOW_ENTER_NORMAL_TWICE
    
    Example TJA1041:
    #define CANTRCV_30_GENERICCAN_SUPPORTS_MODE_STANDBY
    #define CANTRCV_30_GENERICCAN_SUPPORTS_MODE_SLEEP
    #define CANTRCV_30_GENERICCAN_INIT_VIA_NORMAL_MODE
    #define CANTRCV_30_GENERICCAN_ALLOW_ENTER_NORMAL_TWICE
*/
<Your_Trcv_Code>

/* ********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 * ********************************************************************************************************************/
#define CANTRCV_30_GENERICCAN_IS_INIT                         1
#define CANTRCV_30_GENERICCAN_IS_NOT_INIT                     0

/* ********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 * ********************************************************************************************************************/
#define CanTrcv_EnterCritical() SchM_Enter_CanTrcv_30_GenericCan_CANTRCV_30_GENERICCAN_EXCLUSIVE_AREA_0()
#define CanTrcv_LeaveCritical() SchM_Exit_CanTrcv_30_GenericCan_CANTRCV_30_GENERICCAN_EXCLUSIVE_AREA_0()

#if (CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON)
# if (CANTRCV_30_GENERICCAN_PROD_ERROR_DETECT == STD_ON)
#  define CanTrcv_DemReportError(eventId)        ((void)Dem_ReportErrorStatus((eventId), (DEM_EVENT_STATUS_FAILED)))
# endif
#endif

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
# define CanTrcv_DetReportError(apiId, errorId)  ((void)Det_ReportError((CANTRCV_30_GENERICCAN_MODULE_ID), (CANTRCV_30_GENERICCAN_INSTANCE_ID), (apiId), (errorId)))
#endif

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
#  define CanTrcv_ReportErrorNoTrcvControl(apiId) CanTrcv_DetReportError( (apiId), CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL)
#endif

/* If no errors should be reported */
#if defined(CanTrcv_ReportErrorNoTrcvControl)
#else
# define CanTrcv_ReportErrorNoTrcvControl(apiId)
#endif

# define CANTRCV_30_GENERICCAN_SF_WUP_BUS  0x0001u
# define CANTRCV_30_GENERICCAN_SF_WUP_PIN  0x0002u
# define CANTRCV_30_GENERICCAN_SF_WUP_POR  0x0004u
# define CANTRCV_30_GENERICCAN_SF_ERR_BUS  0x0100u
# define CANTRCV_30_GENERICCAN_SF_ERR_SYS  0x0200u
# define CANTRCV_30_GENERICCAN_SF_ERR_CFG  0x0400u

# define CANTRCV_30_GENERICCAN_GET_SF_INIT (CANTRCV_30_GENERICCAN_SF_WUP_BUS|CANTRCV_30_GENERICCAN_SF_WUP_PIN|CANTRCV_30_GENERICCAN_SF_WUP_POR|CANTRCV_30_GENERICCAN_SF_ERR_SYS|CANTRCV_30_GENERICCAN_SF_ERR_CFG)
# define CANTRCV_30_GENERICCAN_GET_SF_SETOPMODE (CANTRCV_30_GENERICCAN_SF_ERR_SYS|CANTRCV_30_GENERICCAN_SF_ERR_CFG)
# define CANTRCV_30_GENERICCAN_GET_SF_CBWAKEUPBYBUS (CANTRCV_30_GENERICCAN_SF_WUP_BUS|CANTRCV_30_GENERICCAN_SF_WUP_PIN|CANTRCV_30_GENERICCAN_SF_ERR_SYS)
# define CANTRCV_30_GENERICCAN_GET_SF_MAINFUNCTIONDIAGNOSTICS (CANTRCV_30_GENERICCAN_SF_ERR_BUS)
# define CANTRCV_30_GENERICCAN_GET_SF_MAINFUNCTION (CANTRCV_30_GENERICCAN_SF_WUP_PIN|CANTRCV_30_GENERICCAN_SF_WUP_BUS)

/* The macro CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON returns true if the
   given wakeup reason indicates an external wakeup event */
#if !defined(CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON)
#  define CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON(wuReason)               \
                        ((wuReason) == CANTRCV_30_GENERICCAN_WU_BY_PIN)    || \
                        ((wuReason) == CANTRCV_30_GENERICCAN_WU_BY_BUS)
#endif /* !defined(CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON) */

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
# define CANTRCV_30_GENERICCAN_IS_INITIALIZED(channel) \
                  ( ( CanTrcv_30_GenericCan_IsInitialized == CANTRCV_30_GENERICCAN_IS_INIT) &&  \
                    ( CanTrcv_30_GenericCan_Prob[channel].isInit == CANTRCV_30_GENERICCAN_IS_INIT) )
#endif

/*  CanTrcv_30_GenericCan_State...
    -------------------------------
    
    These macros shall return true if the specified state of the underlying CAN Transceiver hardware is detected.
    
    Parameters:
      CanTrcv_30_GenericCan_StatusFlagsType statusFlags - Contains the current statusFlags
      
    Return:
      Return true if the state is detected in statusFlags.
      
    The macros shall check the following:
   
    CanTrcv_30_GenericCan_State...
    ...WupIsBus                       Check if a wakup by bus is detected (pattern OR frame)
    ...WupIsPin                       Check if a wakeup by pin is detected
    ...WupIsPor                       Check if a wakeup by power-on-reset is detected
    ...WupIsValid                     Check if a valid wakeup is detected (usually WUP+PIN)
    
    ...ErrIsBus                       Check if a bus error (e.g. dominant state) is detected
    ...ErrIsSys                       Check if SYSERR is detected
    ...ErrIsCfg                       Check if a configuration error occured
    
    If the underlying transceiver hardware does not support one of these flags the corresponding
    macro shall always return false.
*/
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
/*
    Example:
      When using DIO interface:
      =========================
        #define CanTrcv_30_GenericCan_StateWupIsBus(statusFlags)  ( (statusFlags.PinSTB == STD_LOW) && (statusFlags.PinERR == STD_LOW) )
*/
#endif

# define CanTrcv_30_GenericCan_StateWupIsBus(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateWupIsPin(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateWupIsPor(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateWupIsValid(statusFlags) ( CanTrcv_30_GenericCan_StateWupIsBus(statusFlags) || \
                                                               CanTrcv_30_GenericCan_StateWupIsPin(statusFlags) )

# define CanTrcv_30_GenericCan_StateErrIsBus(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateErrIsSys(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateErrIsCfg(statusFlags)   ( <Your_Trcv_Code> )

/* ********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 * ********************************************************************************************************************/

/*  CanTrcv_30_GenericCan_StatusFlagsType
    --------------------------------------
    
    Specify the type of the statusFlags. This type is used for CanTrcv_30_GenericCan_Prob[index].statusFlags.
    The type specified will be used with the "CanTrcv_30_GenericCan_State.." macros to evalute the current
    state of the underlying transceiver hardware.
       
    Example:
      typedef struct {
        uint8 DIAG1;
        uint8 DIAG2;
      } CanTrcv_30_GenericCan_StatusFlagsType;
      
      OR
      
      typedef uint8 CanTrcv_30_GenericCan_StatusFlagsType;
*/    
typedef <Your_Trcv_Code> CanTrcv_30_GenericCan_StatusFlagsType; /* PRQA S 3448 */ /* MD_CanTrcv_30_GenericCan_3448 */

/* The following type includes CAN transceiver channel specific settings. */
typedef struct
{
  /* ---- Common attributes ---- */

  /* Last detected wakeup reason */
  CanTrcv_30_GenericCan_TrcvWakeupReasonType wakeUpReason;

  /* Specifies whether or not to report wakeups */
  CanTrcv_30_GenericCan_TrcvWakeupModeType wakeUpReport;

  /* Holds the last read statusFlags.. only valid if statusFlagsRdy == TRUE */
  CanTrcv_30_GenericCan_StatusFlagsType statusFlags;

  /* Specifies whether the transceiver is initialized */
  uint8 isInit;

  /* HW specific attributes */
  /*  AdditionalProbType
      ------------------
      
      Add channel specific data here. The members will be part of CanTrcv_30_GenericCan_Prob[index].
      
      Example:
        uint8   currentTransceiverState;
  */    
  
  /*  
      <Your_Trcv_Code>
  */

} CanTrcv_30_GenericCan_Prob_Type; /* PRQA S 3448 */ /* MD_CanTrcv_30_GenericCan_3448 */

/* ********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 * ********************************************************************************************************************/

#define CANTRCV_30_GENERICCAN_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
 /*
 * Specifies whether the CanTrcv module is initialized.
 */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
STATIC VAR(uint8, CANTRCV_30_GENERICCAN_VAR) CanTrcv_30_GenericCan_IsInitialized = CANTRCV_30_GENERICCAN_IS_NOT_INIT;
# endif

#define CANTRCV_30_GENERICCAN_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CANTRCV_30_GENERICCAN_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*
 * In following variable CAN transceiver channel specific settings are stored e.g. the wake-up reason or the current operating mode. 
 * For more information see the description of type: CanTrcv_30_GenericCan_Prob_Type.
 */
VAR(CanTrcv_30_GenericCan_Prob_Type, CANTRCV_30_GENERICCAN_VAR) CanTrcv_30_GenericCan_Prob[CANTRCV_30_GENERICCAN_MAX_CHANNEL]; /* PRQA S 3408 */ /* MD_CanTrcv_30_GenericCan_3408 */

/*  LocalData 
    ---------
    Place additional local (static) data here
*/
/* 
  <Your_Trcv_Code>
*/

#if (CANTRCV_30_GENERICCAN_USE_INIT_POINTER == STD_ON)
/* In following variable the identity specific configuration is stored during the initialization of the CAN transceiver driver in case of a Post-build selectable configuration. */
/* PRQA S 3218 1 */ /* MD_CanTrcv_30_GenericCan_8.7 */
STATIC P2CONST(CanTrcv_30_GenericCan_ConfigType, CANTRCV_30_GENERICCAN_VAR, CANTRCV_30_GENERICCAN_CONST) CanTrcv_30_GenericCan_ConfigDataPtr;
#endif

#define CANTRCV_30_GENERICCAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CANTRCV_30_GENERICCAN_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* ********************************************************************************************************************
 *  LOCAL FUNCTIONS
 * ********************************************************************************************************************/
/* Local function declarations */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReportWakeup
 **********************************************************************************************************************/
/*! \brief       Report the wake-up reason to the upper layer.
 *  \details     This function has no service id.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      -
 *  \pre         -
 *  \context     Called by transceiver driver within: LL-specific.
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      -
 *  \trace       CREQ-102476, CREQ-102483, CREQ-102480, CREQ-102474, CREQ-102486
 **********************************************************************************************************************/
LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReportWakeup(uint8 CanTrcvIndex);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin
 **********************************************************************************************************************/
/*! \brief       LL: LL-specific channel initialization.
 *  \details     This function is called for ALL channels and has no service id.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      -
 *  \pre         -
 *  \context     Called by transceiver driver within: CanTrcv_30_GenericCan_Init().
 *  \reentrant   TRUE only for different transceiver channels (index)
 *  \synchronous TRUE
 *  \config      -
 *  \trace       CREQ-102476
 **********************************************************************************************************************/
LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin(uint8 CanTrcvIndex);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitChannel
 **********************************************************************************************************************/
/*! \brief       LL: LL-specific channel initialization.
 *  \details     This function is called only for "used" (== activated) channels and has no service id.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      -
 *  \pre         -
 *  \context     Called by transceiver driver within: CanTrcv_30_GenericCan_Init().
 *  \reentrant   TRUE only for different transceiver channels (index)
 *  \synchronous TRUE
 *  \config      -
 *  \trace       CREQ-102476
 **********************************************************************************************************************/
LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_LL_InitChannel(uint8 CanTrcvIndex);

/*  LocalFunctionDecl 
    -----------------
    Add declarations of local (static) functions here.
*/
/* 
  <Your_Trcv_Code>
*/

/* ********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 * ********************************************************************************************************************/

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_InitMemory
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_InitMemory(void)
{
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #100 Mark driver as uninitialized. */
  CanTrcv_30_GenericCan_IsInitialized = CANTRCV_30_GENERICCAN_IS_NOT_INIT;
#endif
}

/**********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin(uint8 CanTrcvIndex)
{
  /* #100 Initialize the CAN transceiver channel-specific settings (independent active or inactive one) (LL-specific). */
  /*  InitMemory_Channel_Begin 
      ------------------------
      Init additional channel-specific memory here
  */
  /* 
    <Your_Trcv_Code>
  */ 
}

/**********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitChannel
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_LL_InitChannel(uint8 CanTrcvIndex)
{
  /*  Init_Channel_Begin 
      ------------------
      Add declaration of local variables here.
      
      Restrictions:
      - DO NOT access the hardware
      
      Example:
        uint8 localVariable1;
  */
  /* 
    <Your_Trcv_Code>
  */ 

  /* #100 Initialize the CAN transceiver channel-specific settings (only active one) (LL-specific). */
  /*  Init_Channel
      ------------
      Add some channel specific initialization here.
      
      Restrictions:
      - DO NOT modify the operation mode
      - DO NOT initialize ID/MASK/DATA/DLC/CODE (for CanTrcv with selective wakeup support)
      - DO NOT use any asynchronous DIO/SPI API to access the hardware
  */
  /* 
    <Your_Trcv_Code>
  */ 
}

/**********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_Init
 **********************************************************************************************************************/
/*! Internal comment removed.
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
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_Init( P2CONST(CanTrcv_30_GenericCan_ConfigType, AUTOMATIC, CANTRCV_30_GENERICCAN_CONST) ConfigPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
/* ESCAN00076622 */
#if ((CANTRCV_30_GENERICCAN_USE_INIT_POINTER == STD_ON) && (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON))
  /* #10 Check if parameter ConfigPtr is valid. */
  if (ConfigPtr == NULL_PTR)
  {
    /* SREQ-7552 */
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  else
#endif
  {
    uint8_least index;

    /*  Init_Begin 
        ----------------
        
        Global initialization tasks, that are not specific for a channel. You may add additional local variables here.
    */
    /* 
      <Your_Trcv_Code>
    */ 

#if (CANTRCV_30_GENERICCAN_USE_INIT_POINTER == STD_ON)
    /* #100 Store the configuration pointer. */
    CanTrcv_30_GenericCan_ConfigDataPtr = ConfigPtr;
#endif

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
    /* #110 Set transceiver driver as initialized at all. */
    CanTrcv_30_GenericCan_IsInitialized = CANTRCV_30_GENERICCAN_IS_INIT;
#endif

    /* #120 Run through all transceiver channels and initialize them.*/
    for(index = 0; index < CANTRCV_30_GENERICCAN_MAX_CHANNEL; ++index)
    {

      /* Initialize variables */

      /* Initialize LL */
      CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin((uint8)index);

      /* #130 Set the channel states concerning: initialization, wake-up report and wake-up reason to default values. */
      CanTrcv_30_GenericCan_Prob[index].isInit = CANTRCV_30_GENERICCAN_IS_NOT_INIT;
      CanTrcv_30_GenericCan_Prob[index].wakeUpReport = CANTRCV_30_GENERICCAN_WU_ENABLE;
      CanTrcv_30_GenericCan_Prob[index].wakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR;

      /* #140 Set the channel state concerning: request state to default values (only SPI-interface or SBC-interface with PN). */

      /* ----- Implementation ----------------------------------------------- */

      /* #190 Initialize next parameters only for active transceiver channels. */
      if( CanTrcvCfg_IsChannelUsed(index) )
      {

# if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) 
        Std_ReturnType getStatusFlagRetVal = E_NOT_OK;
# endif

        Std_ReturnType modeSet = E_NOT_OK;

        /* #200 If no wake-up event occurred during initialization the default reason is RESET. */
        CanTrcv_30_GenericCan_Prob[index].wakeUpReason = CANTRCV_30_GENERICCAN_WU_RESET;

        /* #220 Do additional checks: wake-up event, HW status, PN configuration. (critical section is used). */
        CanTrcv_EnterCritical();
#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) 
        /* SREQ-7551 */

          /* #230 Check general status flags. If fails report error. */
        /*  GetStatusFlags
            --------------
            Request all status flags (SYSERR, POR, WUP, WUF, LWUP, ...) from the underlying transceiver hardware. 
            Set getStatusFlagRetVal = E_OK if the request was successful. The flags are evaluated later with the CanTrcv_30_GenericCan_State*
            macros.
            
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO interface:
            =========================
              Store the current level of the DIO pins to statusFlags. 
              
              Example TJA1041:
                statusFlags.PinERR = Dio_ReadChannel(CanTrcv_30_GenericCan_GetERROfDioConfiguration(index));
                statusFlags.PinSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(index));
                getStatusFlagRetVal = E_OK;
        */
        #endif
        <Your_Trcv_Code>
        if ( (getStatusFlagRetVal == E_NOT_OK) )
        {
          /* SREQ-10166,SREQ-7553  */
          CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_INIT_ID);
        }
#endif /* CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON || CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */

        /* #240 Determine whether a wake-up event (BY_BUS, BY_PIN) occurred or NOT. If occurred store it. */
#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
        if ( (getStatusFlagRetVal == E_OK) )
        {
          if ( CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[index].statusFlags) )
          {
            /* A wakeup occurred. The reason is: BY_BUS */
            CanTrcv_30_GenericCan_Prob[index].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_BUS;
          }
          else if (CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[index].statusFlags))
          {
            /* A wakeup occurred. The reason is: BY_PIN */
            CanTrcv_30_GenericCan_Prob[index].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_PIN;
          }
          else
          {
            /* Nothing. */
          }
        }
#endif /* CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON */

        CanTrcv_LeaveCritical();

        /* Do some generic initialization of LL */
        CanTrcv_30_GenericCan_LL_InitChannel((uint8)index);

        /* #260 Report the detected wake-up event to the upper layer. */
        CanTrcv_30_GenericCan_ReportWakeup((uint8)index);

        /* #270 Set the operating mode to NORMAL. In case of SPI-interface always synchronous. (critical section used) */
        CanTrcv_EnterCritical();

#if (CANTRCV_30_GENERICCAN_USE_ICU == STD_ON)
        if ( CanTrcvCfg_IsIcuChannelSet(index) ) /* SREQ-7664 */
        {
          Icu_DisableNotification( CanTrcvCfg_GetIcuChannel(index) ); /* SREQ-7668 */
        }
#endif

        /*  SetOpModeNormal
            ---------------
            
            Set the underlying transceiver hardware to normal operation mode by changing
            the Dio Levels.
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO intrface:
            ========================
              Example for TJA1041:
                Dio_WriteChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(index),STD_HIGH);
                Dio_WriteChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(index),STD_HIGH);
                modeSet = E_OK;
        */
        #endif
        <Your_Trcv_Code>
        
        modeSet = E_OK;

        CanTrcv_LeaveCritical();

        /* #300 Check if setting of operating mode was successful. */
        if (modeSet == E_OK) /* PRQA S 3355, 3358 */ /* MD_CanTrcv_30_GenericCan_335X, MD_CanTrcv_30_GenericCan_335X */
        {
          /* #330 Mode change was successful: Set current transceiver channel to initialized. */
          CanTrcv_30_GenericCan_Prob[index].isInit = CANTRCV_30_GENERICCAN_IS_INIT;
        }
        else
        { /* PRQA S 3201 */ /* MD_CanTrcv_30_GenericCan_3201 */
          /* SREQ-10166, SREQ-7553 */
          /* #340 Mode change was NOT successful: Report error. */
          CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_INIT_ID);
        }
      } /* CanTrcvChannelUsed == TRUE */
    } /* for (...) */
  } /* if (ConfigPtr == NULL_PTR) .. else */

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR) /* PRQA S 3356, 3359 */ /* MD_CanTrcv_30_GenericCan_335X, MD_CanTrcv_30_GenericCan_335X */
  { /* PRQA S 3201 */ /* MD_CanTrcv_30_GenericCan_3201 */
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_INIT_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId);
#endif
  
#if (CANTRCV_30_GENERICCAN_USE_INIT_POINTER == STD_OFF)
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(ConfigPtr);
#endif
}  /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetOpMode
 **********************************************************************************************************************/
/*! Internal comment removed.
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetOpMode(uint8 CanTrcvIndex, CanTrcv_TrcvModeType OpMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  Std_ReturnType modeSet = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

  CanTrcv_30_GenericCan_TrcvModeType currentOpMode;

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* SREQ-7568, SREQ-10175 */  
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if( !CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
  {
    /* SREQ-7568, SREQ-10175 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if( !CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex) )
  {
    /* SREQ-7568 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute transition of operating mode only if transceiver channel (CanTrcvIndex) is active. */
    if( CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
#endif
    {
      /* #110 Check if the current operating mode equals the requested one. */
      (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode);

#if defined (CANTRCV_30_GENERICCAN_ALLOW_ENTER_NORMAL_TWICE)
      /* If OpMode is the same as the currentOpMode (except for NORMAL MODE), return E_OK */
      if ( (currentOpMode == OpMode) && (OpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL) )
#else
      if (currentOpMode == OpMode) /* SREQ-7661 */
#endif
      {
        /* SREQ-10171,SREQ-10170,SREQ-10169 */
        returnVal = E_OK;

      }
      else
      {
        /* #160 The requested one does NOT equal the current one. Execute the requested mode change. (critical section is used) */
        /* SREQ-10168,SREQ-7567 */
        switch(OpMode)
        {
          /* #200 Mode change request to NORMAL. */
          case CANTRCV_30_GENERICCAN_OP_MODE_NORMAL:

            (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode);
         
            /* #210 Check whether it is an internal wake-up. If yes store the wake-up reason. */         
            if(( (currentOpMode == CANTRCV_30_GENERICCAN_OP_MODE_SLEEP) || \
                 (currentOpMode == CANTRCV_30_GENERICCAN_OP_MODE_STANDBY) ) && \
                 (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason == CANTRCV_30_GENERICCAN_WU_ERROR))
            {
              /* Transceiver is switched from STANDBY/SLEEP -> NORMAL and has not detected any wakeup.
                 This is considered as internal wakeup. */
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_INTERNALLY;
            }
            
            CanTrcv_EnterCritical();

            /* #220 If ICU is configured, disable ICU. */
# if (CANTRCV_30_GENERICCAN_USE_ICU == STD_ON)
            if ( CanTrcvCfg_IsIcuChannelSet(CanTrcvIndex) ) /* SREQ-7664 */
            {
              Icu_DisableNotification( CanTrcvCfg_GetIcuChannel(CanTrcvIndex) ); /* SREQ-7668 */
            }
# endif
            
            {
              /* #240 No mode request is currently pending. Store the current mode request. (only SPI-interface) */
              /* #250 Execute operating mode change request to NORMAL. */
              /*  SetOpModeNormal
                  ---------------
                  
                  Set the underlying transceiver hardware to normal operation mode by changing
                  the Dio Levels.
              */
              #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
              /*  When using DIO intrface:
                  ========================
                    Example for TJA1041:
                      Dio_WriteChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex),STD_HIGH);
                      Dio_WriteChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(CanTrcvIndex),STD_HIGH);
                      modeSet = E_OK;
              */
              #endif
              <Your_Trcv_Code>
              
              modeSet = E_OK;
            }

            CanTrcv_LeaveCritical();
            returnVal = E_OK;
            break;

          case CANTRCV_30_GENERICCAN_OP_MODE_STANDBY:
             /* #300 Mode change request to STANDBY. (only if supported by HW) */
#if defined (CANTRCV_30_GENERICCAN_SUPPORTS_MODE_STANDBY)

            (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode);

            /* #310 Mode transition to STANBDY is only possible from NORMAL. Otherwise DET error is reported. */
            if(currentOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL) /* SREQ-10169,SREQ-7567 */
            {
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
              /* SREQ-7568,SREQ-10173 */
              errorId = CANTRCV_30_GENERICCAN_E_TRCV_NOT_NORMAL;
# endif
            }
            else
            {
              /* #320 Current mode is NORMAL: Execute mode request. */
              CanTrcv_EnterCritical();

              /* #330 If ICU is configured, disable ICU. */
# if (CANTRCV_30_GENERICCAN_USE_ICU == STD_ON)
              if ( CanTrcvCfg_IsIcuChannelSet(CanTrcvIndex) ) /* SREQ-7664 */
              {
                Icu_EnableNotification( CanTrcvCfg_GetIcuChannel(CanTrcvIndex) ); /* SREQ-7667 */
              }
# endif
              /* #340 Reset wake-up reason. */
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR;

              {
                /* #360 No mode request is currently pending. Store the mode request. (only SPI-interface) */
                /* #370 Execute operating mode change request to STANDBY. */
                /*  SetOpModeStandby
                    ----------------
                    Set the underlying transceiver hardware to operation mode standby.
                */
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
                /*  When using DIO interface:
                    =========================
                      Example for TJA1041:
                        Dio_WriteChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(CanTrcvIndex),STD_LOW);
                        Dio_WriteChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex),STD_LOW);
                        modeSet = E_OK;
                */
                #endif
                <Your_Trcv_Code>
                
                modeSet = E_OK;
              }

              CanTrcv_LeaveCritical();
              returnVal = E_OK;
            }
#else
              /* Standby mode is not supported */
            returnVal = E_NOT_OK; /* SREQ-7567 */
#endif /* CANTRCV_30_GENERICCAN_SUPPORTS_MODE_STANDBY */
            break;

          case CANTRCV_30_GENERICCAN_OP_MODE_SLEEP:
            /* #400 Mode change request to SLEEP. (only if supported by HW) */
#if defined (CANTRCV_30_GENERICCAN_SUPPORTS_MODE_SLEEP)

            (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode);

            /* #410 Mode transition to SLEEP is only possible from STANDBY. Otherwise DET error is reported. */
            if(currentOpMode != CANTRCV_30_GENERICCAN_OP_MODE_STANDBY) /* SREQ-10171,SREQ-7567 */
            {
#  if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
              /* SREQ-10173,SREQ-10174 */
              errorId = CANTRCV_30_GENERICCAN_E_TRCV_NOT_STANDBY;
#  endif
            }
            else
            {
              CanTrcv_EnterCritical();

              {
                /* #430 No mode request is currently pending. Store the mode request. (only SPI-interface) */
                /* #440 Execute operating mode change request to SLEEP. */
                /*  SetOpModeSleep
                    ----------------
                    Set the underlying transceiver hardware to operation mode sleep.
                    
                */  
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)  
                /*  When using DIO interface:
                    =========================
                      Example for TJA1041:
                        Dio_WriteChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(CanTrcvIndex),STD_LOW);
                        Dio_WriteChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex),STD_LOW);
                */
                #endif
                <Your_Trcv_Code>
                
                modeSet = E_OK;
              }

              CanTrcv_LeaveCritical();
              returnVal = E_OK;
            }
#else
            /* Sleep mode is not supported */
            returnVal = E_NOT_OK; /* SREQ-7567 */
#endif /* CANTRCV_30_GENERICCAN_SUPPORTS_MODE_SLEEP */
            break;

          default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
            /* SREQ-7568 */
            /* #40 Check if parameter OpMode is valid. */
            errorId = CANTRCV_30_GENERICCAN_E_PARAM_TRCV_OP_MODE;
#endif
            break;
          } /* switch() */
      } /* currentOpMode == OpMode */

      /* #600 If mode request executed successfully then inform the CanIf else report error. */
      if (returnVal == E_OK)
      {
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode);

        if(currentOpMode == OpMode)
#  endif
        {
          /* SREQ-10165 */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
          CanIf_30_GenericCan_TrcvModeIndication(CanTrcvIndex, currentOpMode); /* SREQ-10165  */
#  endif
        }
        else
        {
          /* SREQ-10172, SREQ-7571 */
          CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_SETOPMODE_ID);

          returnVal = E_NOT_OK;
        } /* currentOpMode != OpMode */
      } /* returnVal == E_OK */
    } /* channelUsed == TRUE */
  }

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_SETOPMODE_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId);
#endif

  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(modeSet);
  return returnVal;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetOpMode
 **********************************************************************************************************************/
/*! Internal comment removed.
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
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetOpMode(uint8 CanTrcvIndex, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) OpMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

#if defined(CANTRCV_30_GENERICCAN_INTERFACE_DIO)
  /*  DioPins_GetOpMode
      -----------------
  
      Specify the needed Dio pins for reading the transceivers current operation 
      mode. The pins have to be specified with Dio_LevelType type.
      
      Example for TJA1041:
      Dio_LevelType CanTrcvSTB;
      Dio_LevelType CanTrcvEN;
  */
  <Your_Trcv_Code>
# endif

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter OpMode is valid. */
  if(OpMode == NULL_PTR)
  {
    /* SREQ-7540 */
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  /* #20 Check if parameter CanTrcvIndex is valid. */
  else if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* SREQ-7540,SREQ-10179  */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is active. */
  else if( !CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
  {
    /* SREQ-7540,SREQ-10179  */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #40 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if( !CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex) )
  {
    /* SREQ-7540 */
    errorId =  CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
    if( CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
#endif
    {

#if defined(CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
      /* #200 
         DIO-interface: Read the mode from status of PINs of underlying HW.
         SPI-interface: Read the mode from state variable. 
         SBC-interface: Read the mode via SBC module API. 
         (critical section used)
       */
      CanTrcv_EnterCritical();

      /*  GetPins_GetOpMode
          -----------------
      */  
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)    
      /*  When using DIO interface:
          =========================
            Read the DioChannel Levels for the Trcv pins needed to obtain the current
            operation mode. You may use CanTrcv_30_GenericCan_Get<NameOfPin>OfDioConfiguration(CanTrcvIndex)
            to get the configured Dio pins.
            Store the values to the local Dio_LevelType variables specified in 
            DioPins_GetOpMode. 
            
            Example for TJA1041 
              CanTrcvSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex));
              CanTrcvEN  = Dio_ReadChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(CanTrcvIndex));
      */
      #endif
      <Your_Trcv_Code>
      
      returnVal = E_OK;

      CanTrcv_LeaveCritical();

      if (returnVal == E_OK) /* PRQA S 3355,3358 */ /* MD_CanTrcv_30_GenericCan_13.7 */
#endif
      {
        /* SREQ-10177,SREQ-7539 */

        /*  CheckOpmodeNormal
            -----------------
            Check if normal mode is reached.
            
            This code must have the following format
            
              if(<condition for normal mode>)
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO interface
            ========================
              Use the local Dio_LevelType variables specified in DioPins_GetOpMode and
              read in GetPins_GetOpMode to determine the current operation mode.
            
              Example for TJA1041
                if((CanTrcvSTB == STD_HIGH) && (CanTrcvEN == STD_HIGH))
        */ 
        #endif
        <Your_Trcv_Code>
        {
          /* CAN transceiver is in normal operation mode */
          *OpMode = CANTRCV_30_GENERICCAN_OP_MODE_NORMAL;
          returnVal = E_OK;
        }

#if defined (CANTRCV_30_GENERICCAN_SUPPORTS_MODE_SLEEP)
        /*  CheckOpmodeSleep
            -----------------
            Check if sleep mode is reached.
            
            This code must have the following format
            
              if(<condition for normal mode>)
              
            If sleep mode is not supported, you should place if(0) here and remove
            the define from SupportedModes      
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO interface
            ========================
              Use the local Dio_LevelType variables specified in DioPins_GetOpMode and
              read in GetPins_GetOpMode to determine the current operation mode.
            
              Example for TJA1041
                if((CanTrcvSTB == STD_LOW) && (CanTrcvEN == STD_HIGH))
        */
        #endif
        <Your_Trcv_Code>
        {
          /* CAN transceiver is in sleep mode */
          *OpMode = CANTRCV_30_GENERICCAN_OP_MODE_SLEEP;
          returnVal = E_OK;
        }

#endif /* CANTRCV_30_GENERICCAN_SUPPORTS_MODE_SLEEP */

#if defined (CANTRCV_30_GENERICCAN_SUPPORTS_MODE_STANDBY)
        /*  CheckOpmodeStandby
            -----------------
            Check if standby mode is reached.
            
            This code must have the following format
            
              if(<condition for normal mode>)
              
            If standby mode is not supported, you should place if(0) here and remove
            the define from SupportedModes      
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*
            When using DIO interface
            ========================
              Use the local Dio_LevelType variables specified in DioPins_GetOpMode and
              read in GetPins_GetOpMode to determine the current operation mode.
            
              Example for TJA1041
                if((CanTrcvSTB == STD_LOW) && (CanTrcvEN == STD_LOW))
        */
        #endif
        <Your_Trcv_Code>
        {
          /* CAN transceiver is in standby mode */
          *OpMode = CANTRCV_30_GENERICCAN_OP_MODE_STANDBY;
          returnVal = E_OK;
        }
#endif /* CANTRCV_30_GENERICCAN_SUPPORTS_MODE_STANDBY */
      }

      /* #210 If reading of operating mode NOT succeeded report error. */
      if (returnVal == E_NOT_OK) /* PRQA S 3356,3359 */ /* MD_CanTrcv_30_GenericCan_13.7 */
      { /* PRQA S 3201 */ /* MD_CanTrcv_30_GenericCan_3201 */
        /* SREQ-10178, SREQ-7541 */
        CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_GETOPMODE_ID);
      }
    } /* Channel is used */
  } /* DET checks passed */
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_GETOPMODE_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId);
#endif

 return returnVal;
}  /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetBusWuReason
 **********************************************************************************************************************/
/*! Internal comment removed.
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
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetBusWuReason(uint8 CanTrcvIndex, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) Reason)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE)
  CanTrcv_30_GenericCan_TrcvModeType currentOpMode;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if the return-parameter Reason is valid. */
  if(Reason == NULL_PTR)
  {
    /* SREQ-7537 */
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  /* #20 Check if parameter CanTrcvIndex is valid. */
  else if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* SREQ-10182,SREQ-7537 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is active. */
  else if( !CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
  {
    /* SREQ-10182,SREQ-7537 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #40 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if( !CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex) )
  {
    /* SREQ-7537 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
    if( CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
#endif
    {
      /* UREQ00015010 */
      /* #200 Check the wake-up reason for validity. */
#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE)
      if((CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason == CANTRCV_30_GENERICCAN_WU_ERROR)             || \
         (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason == CANTRCV_30_GENERICCAN_WU_NOT_SUPPORTED)       )
      {
        /* #300 Wake-up reason is invalid (either WU_ERROR or WU_NOT_SUPPORTED). This is OK only in case of transceiver channel is in NORMAL mode. */
        (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode);
      
        if(currentOpMode == CANTRCV_30_GENERICCAN_OP_MODE_NORMAL)
        {
          /* #310 Transceiver channel is in NORMAL mode: Return the current wake-up reason. */
          *Reason = CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason;
        }
        else
        {
          /* Being in any mode except NORMAL with WU_ERROR or WU_NOT_SUPPORT is an error */
          /* #320 Transceiver channel is in NOT in NORMAL mode: Return the wake-up reason WU_ERROR. */
          *Reason = CANTRCV_30_GENERICCAN_WU_ERROR;
        }

        if(*Reason == CANTRCV_30_GENERICCAN_WU_ERROR)
        {
          /* SREQ-10181,SREQ-7538 */
          /* #330 Report error if the wake-up reason to be returned is invalid (WU_ERROR). */
          CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_GETBUSWUREASON_ID);
        }
      }
      else
      {
        /* #400 Wake-up reason is valid (neither WU_ERROR nor WU_NOT_SUPPORTED). Return the current wake-up reason.*/
        *Reason = CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason;
        returnVal = E_OK;
      }
#endif
#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT == CANTRCV_30_GENERICCAN_WAKEUP_NONE)
      *Reason = CANTRCV_30_GENERICCAN_WU_NOT_SUPPORTED;
#endif
    } /* IsChannelUsed */
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_GETBUSWUREASON_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId);
#endif
  
  return returnVal;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetWakeupMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetWakeupMode(uint8 CanTrcvIndex, CanTrcv_TrcvWakeupModeType TrcvWakeupMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* SREQ-10191,SREQ-7565  */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if( !CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
  {
    /* SREQ-10191,SREQ-7565  */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex) )
  {
    /* SREQ-7565 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE)
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
    if( CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
# endif
    {
      /* #200 Set the requested wake-up mode. */
      switch(TrcvWakeupMode)
      {
        case CANTRCV_30_GENERICCAN_WU_ENABLE:
             /* SREQ-7541: -> "Notification" not specified! */
             /* SREQ-10185,SREQ-7564 */
             CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReport = CANTRCV_30_GENERICCAN_WU_ENABLE;

             /* #300 In addition: If requested wake-up mode is WU_ENABLE report if wake-up event(s) is/are pending. */
             CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);

             returnVal = E_OK;
             break;

        case CANTRCV_30_GENERICCAN_WU_DISABLE:
             /* SREQ-10186,SREQ-7564 */
             CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReport = CANTRCV_30_GENERICCAN_WU_DISABLE;

             returnVal = E_OK;
             break;

        case CANTRCV_30_GENERICCAN_WU_CLEAR:
             /* SREQ-10187,SREQ-7564 */
             /* #500 In addition: If requested wake-up mode is WU_CLEAR clear the wake-up reason and all detected wake-up events. */
             CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR;
             returnVal = E_OK;

             break;
        default:  /* PRQA S 2018 */ /* MD_MSR_14.1 */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
             /* #40 Check if parameter TrcvWakeupMode is valid. */
             errorId = CANTRCV_30_GENERICCAN_E_PARAM_TRCV_WAKEUP_MODE; /* SREQ-7565 */
# endif
             break;
      }
    }
#endif /* CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE */
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_SETWAKEUPMODE_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId);
#endif  
  
#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT == CANTRCV_30_GENERICCAN_WAKEUP_NONE)
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(TrcvWakeupMode);
# if(CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(CanTrcvIndex);
# endif
#endif
  /* SREQ-10190  Not implemented as no need to query HW at this place. */

  return returnVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckWakeup
 **********************************************************************************************************************/
/*! Internal comment removed.
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
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckWakeup(uint8 CanTrcvIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* SREQ-7528,SREQ-10204 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if( !CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
  {
    /* SREQ-7528,SREQ-10204 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex) )
  {
    /* SREQ-7528,SREQ-10203 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active and wake-up detection is enabled. (critical section used) */
    if( CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
# endif
    {
      if( CanTrcvCfg_IsWakeupByBusUsed(CanTrcvIndex) && \
          (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReport == CANTRCV_30_GENERICCAN_WU_ENABLE) )
      {
        CanTrcv_EnterCritical();

        /* #200 Check if there are any wake-up events pending in HW. */
        if ( CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason) )
        {
          /* #300 Any wake-up event is pending: Check if currently an operating mode change is requested. (only asynchronous SPI-interface) */
          {
            /* #340 Operating mode STANDBY / SLEEP: Report the detected wake-up event to the EcuM. */
            CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);

            returnVal = E_OK; /* wake-up detected  */
          }
        }
        else
        {
          /* #400 No pending wake-up flags. Request the underlying HW for wake-up flags. */

          /*  GetStatusFlags
              --------------
              Request all status flags (SYSERR, POR, WUP, WUF, LWUP, ...) from the underlying transceiver hardware. 
              Set returnVal = E_OK if the request was successful. The flags are evaluated later with the CanTrcv_30_GenericCan_State*
              macros.
              
          */
          #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
          /*  When using DIO interface:
              =========================
                Store the current level of the DIO pins to statusFlags. 
                
                Example TJA1041:
                  statusFlags.PinERR = Dio_ReadChannel(CanTrcv_30_GenericCan_GetERROfDioConfiguration(CanTrcvIndex));
                  statusFlags.PinSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex));
                  returnVal = E_OK;
          */
          #endif
          <Your_Trcv_Code>
          /* --> Flags will be processed in GetStatusFlagsIndication + CbWakeupByBusIndication */

          /* #500 If any wake-up event detected determine the wake-up reason (e.g BY_BUS, BY_PIN). (only DIO-interface or SBC-interface) */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
          if ( (returnVal == E_OK) )
          {
            if (CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))
            {
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_BUS;
              CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);

              returnVal = E_OK; /* Wakeup detected */
            }
            else if (CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))
            {
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_PIN;
              CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);

              returnVal = E_OK; /* Wakeup detected */
            }
            else
            {
              returnVal = E_NOT_OK; /* No Wakeup detected */
            }
          }
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_DIO || CANTRCV_30_GENERICCAN_INTERFACE_SBC_COMMENT*/

        }

        CanTrcv_LeaveCritical();

      }
    }
#else
    returnVal = E_NOT_OK;
#endif /* CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED */
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_CHECKWAKEUP_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId);
#endif  
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF) && (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_OFF)
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(CanTrcvIndex);
#endif
  return returnVal;
}  /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

#if (CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON) /* SREQ-7547 */
/* SREQ-7546,SREQ-10183 */
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetVersionInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter VersionInfo is valid. */
  if(VersionInfo == NULL_PTR)
  {
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  else
# endif /* CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* SREQ-7545 */
    /* #100 Retrieve the version information: VENDOR_ID, MODULE_ID, SW_MAJOR-, SW_MINOR- and SW_PATCH-version. */
    VersionInfo->vendorID = CANTRCV_30_GENERICCAN_VENDOR_ID;
    VersionInfo->moduleID = CANTRCV_30_GENERICCAN_MODULE_ID;

    VersionInfo->sw_major_version = CANTRCV_30_GENERICCAN_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = CANTRCV_30_GENERICCAN_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = CANTRCV_30_GENERICCAN_SW_PATCH_VERSION;
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_GETVERSIONINFO_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId);
#endif 
}
#endif /* CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_MainFunction
 **********************************************************************************************************************/
/*! Internal comment removed.
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
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least index;
  
  /* ----- Implementation ----------------------------------------------- */
  /* SREQ-7554 */
  /* #100 Run through all transceiver channels. */
  for(index=0; index < CANTRCV_30_GENERICCAN_MAX_CHANNEL; ++index)
  {
    if (  CanTrcvCfg_IsChannelUsed(index)  )
    {
      if(CanTrcv_30_GenericCan_Prob[index].isInit == CANTRCV_30_GENERICCAN_IS_INIT)
      {
        /* #200 Execute only if transceiver channel is active and initialized. */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_30_GenericCan_TrcvModeType currentOpMode;

        (void)CanTrcv_30_GenericCan_GetOpMode((uint8)index, &currentOpMode);

        /* #300 Execute only if transceiver channel is in power-down mode (STANDBY / SLEEP). (only DIO- or SBC-interface) */
        if ( currentOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL )
#endif
        {
#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) && (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT == CANTRCV_30_GENERICCAN_WAKEUP_POLLING)
          if ( CanTrcvCfg_IsWakeupByBusUsed(index) )
          {
            {
              /* #210 Transceiver channel is in power-down mode (STANDBY / SLEEP) and has wake-up support enabled: Check if a wake-up event occurred. (critical section used) */
              Std_ReturnType retval = E_NOT_OK;

              CanTrcv_TrcvWakeupReasonType wakeupReason = CANTRCV_30_GENERICCAN_WU_ERROR;

              CanTrcv_EnterCritical();
              /* #430 Determine whether a wake-up event occurred or NOT from status flags read via SPI. (only SPI- / SBC-interface) */
              /* Check for status bits */
              {
                 /* #440 If NO most current status flags are available, request them via SPI. (only SPI-interface) */

                 /* No request pending.. its safe to request diagnostic registers here..
                 * unread data will be marked as dirty. This request may be
                 * interrupted by any other request..
                 */
                /*  GetStatusFlags
                    --------------
                    Request all status flags (SYSERR, POR, WUP, WUF, LWUP, ...) from the underlying transceiver hardware. 
                    Set retval = E_OK if the request was successful. The flags are evaluated later with the CanTrcv_30_GenericCan_State*
                    macros.
                    
                */
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
                /*  When using DIO interface:
                    =========================
                      Store the current level of the DIO pins to statusFlags. 
                      
                      Example TJA1041:
                        statusFlags.PinERR = Dio_ReadChannel(CanTrcv_30_GenericCan_GetERROfDioConfiguration(index));
                        statusFlags.PinSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(index));
                        retval = E_OK;
                */
                #endif
                <Your_Trcv_Code>
              }
              if (retval == E_OK)
              {

                /* #460 If most current flags are available process them and if any wake-up event occurred determine the wake-up reason (BY_SYSERR (only PN), BY_BUS or BY_PIN). (only SPI- or SBC-interface) */
                if (CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[index].statusFlags))
                {
                  wakeupReason = CANTRCV_30_GENERICCAN_WU_BY_BUS;
                }
                else if (CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[index].statusFlags))
                {
                  wakeupReason = CANTRCV_30_GENERICCAN_WU_BY_PIN;
                }
                else
                {
                  /* Nothing */
                }

                retval = E_OK;
              }

              if (retval == E_NOT_OK) /* PRQA S 3356,3359 */ /* MD_CanTrcv_30_GenericCan_13.7 */
              { /* PRQA S 3201 */ /* MD_CanTrcv_30_GenericCan_3201 */
                /* #470 If the check for wake-up events fails, report error. (only SPI- / SBC-interface) */
                CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_MAINFUNCTION_ID);
              }

              if (wakeupReason != CANTRCV_30_GENERICCAN_WU_ERROR)
              {
                /* #500 If any wake-up event detected, store the wake-up event. */
                CanTrcv_30_GenericCan_Prob[index].wakeUpReason = wakeupReason;

                if (CanTrcv_30_GenericCan_Prob[index].wakeUpReport == CANTRCV_30_GENERICCAN_WU_ENABLE)
                {
                  /* #510 If any wake-up event detected and wake-up detection is enabled, inform the EcuM to check for wake-up event(s). */
                  EcuM_CheckWakeup( CanTrcvCfg_GetWakeupSource(index) ); /* SREQ-7554 */
                }
              }

#  if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
              CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(retval);
#  endif
              CanTrcv_LeaveCritical();
            } /* (!CanTrcv_30_GenericCan_IsReqFlagPnd(index)) */
          }
# endif /* (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) */
        } /* CanTrcv_30_GenericCan_Prob[index].curOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL */
      } /* CanTrcv_30_GenericCan_Prob[index].isInit != CANTRCV_30_GENERICCAN_IS_INIT */
    } /*  CanTrcvCfg_IsChannelUsed(index)  */
  } /* for ... */
}  /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

# if (CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_MainFunctionDiagnostics
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_MainFunctionDiagnostics(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least index; /* ESCAN00091616 */
  
  /* ----- Implementation ----------------------------------------------- */
  /* SREQ-7556 */
  /* #100 Run through all transceiver channels. */
  for(index = 0; index < CANTRCV_30_GENERICCAN_MAX_CHANNEL; ++index)
  {
    if (CanTrcvCfg_IsChannelUsed(index))
    {
      if (CanTrcv_30_GenericCan_Prob[index].isInit == CANTRCV_30_GENERICCAN_IS_INIT)
      {
        /* #200 Do execute only if transceiver channel is active and initialized. */
        {
          /* #210 Execute only if SPI is NOT busy. (only SPI-interface) */
          Std_ReturnType retval = E_NOT_OK;
#  if (CANTRCV_30_GENERICCAN_PROD_ERROR_DETECT == STD_ON)
          CanTrcv_EnterCritical();

          /* #220 Get diagnostics flags from HW. (critical section used) */
          {
            /* UREQ00014985 */
            /*  GetStatusFlags
                --------------
                Request all status flags (SYSERR, POR, WUP, WUF, LWUP, ...) from the underlying transceiver hardware. 
                Set retval = E_OK if the request was successful. The flags are evaluated later with the CanTrcv_30_GenericCan_State*
                macros.
                
            */
            #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
            /*  When using DIO interface:
                =========================
                  Store the current level of the DIO pins to statusFlags. 
                  
                  Example TJA1041:
                    statusFlags.PinERR = Dio_ReadChannel(CanTrcv_30_GenericCan_GetERROfDioConfiguration(index));
                    statusFlags.PinSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(index));
                    retval = E_OK;
            */
            #endif
            <Your_Trcv_Code>

            if (retval == E_NOT_OK)  /* PRQA S 3355,3358 */ /* MD_CanTrcv_30_GenericCan_13.7 */
            {
              /* #240 If getting of diagnostic flags fails, report error. */
              CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_MAINFUNCTIONDIAGNOSTICS);
            }
          }
          {
            /* #250 If diagnostic flags are available, check if bus error occurred. */
            if (CanTrcv_30_GenericCan_StateErrIsBus(CanTrcv_30_GenericCan_Prob[index].statusFlags))
            {
              /* SREQ-7559 */
              /* #260 If bus error occurred, report error to DEM. */
              CanTrcv_DemReportError(CANTRCV_30_GENERICCAN_E_BUS_ERROR);
            }
          }

          CanTrcv_LeaveCritical();
#  endif /* (CANTRCV_30_GENERICCAN_PROD_ERROR_DETECT == STD_ON) */
        }
      }
    } /* IsChannelUsed */
  } /* FOR-loop: run through all CanTrcv channels */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif /* CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReportWakeup
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReportWakeup(uint8 CanTrcvIndex)
{
  /* ----- Implementation ----------------------------------------------- */
  
  /* #100 If any wake-up event occurred, report it to EcuM. */
  /* #110 Depending on configuration report to EcuM the wake-up source which belongs to occurred wake-up reason (POWER_ON, BY_SYSERR (only PN), BY_BUS, BY_PIN). */
  switch(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason)
  {
  case CANTRCV_30_GENERICCAN_WU_POWER_ON:
# if (CANTRCV_30_GENERICCAN_USE_EXTENDED_WU_SRC == STD_ON)
    /* SREQ-27276 */
    EcuM_SetWakeupEvent( CanTrcvCfg_GetWakeupSourcePor(CanTrcvIndex) );
    break;
# endif
  case CANTRCV_30_GENERICCAN_WU_BY_BUS:
  case CANTRCV_30_GENERICCAN_WU_BY_PIN:
    /* SREQ-7551 */
    EcuM_SetWakeupEvent( CanTrcvCfg_GetWakeupSource(CanTrcvIndex) );
    break;

  default:
    /* No valid or an internal wakeup. Do not report this as wakeup event */
    break;
  }
}

/*  LocalFunctionImpl
    -----------------
    Add implementation of local (static) functions here.
*/
/* 
  <Your_Trcv_Code>
*/

#define CANTRCV_30_GENERICCAN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* **************************************************************************/
/*  MISRA deviations                                                        */
/* **************************************************************************/
/* Justification for module-specific MISRA deviations:

MD_CanTrcv_30_GenericCan_3436:
  Reason: Compatibility define used here.
  Risk: name conflict.
  Prevention: Code review and compiler check for name conflicts.

MD_CanTrcv_30_GenericCan_0715:
  Reason: Complex code structure used and not split in modules or functions to make the code compact and efficent.
  Risk: Hard to understand.
  Prevention: Code inspection and multiple configuration aspects used in component test.

MD_CanTrcv_30_GenericCan_0777:
  Reason: ASR requires that all externals and APIs have the format <Msn>_<xyz>, while <Msn> has the
          format <BswmModuleName>_<VendorId>_<VendorApiInfix> for external hardware. Because of this
          requirement the first chars are same for all external declartations.
  Risk: Non-conforming Code.
  Prevention: Multiple configuration aspects used in component test.

MD_CanTrcv_30_GenericCan_0779
  Reason: Code which is much better and easier to maintain.
  Risk: Compiler and preprocessor do not support identifiers with more than 31 characters.
  Prevention: Test of software with real compiler and preprocessor on real MCU-target.

MD_CanTrcv_30_GenericCan_13.7:
  Reason: Result or value is always true or false depend on configuration aspect and used HW specific implementation
  Risk: Incorrect value return will be ignored.
  Prevention: Code inspection and test of the different variants in the component test.

MD_CanTrcv_30_GenericCan_0883:
  Reason: For development purpose these files are included without header guard.
  Risk: None.
  Prevention: None.

MD_CanTrcv_30_GenericCan_3410:
  Reason: Sometimes, a macro parameter must not be enclosed in () as this would lead to syntax errors.
  Risk: None.
  Prevention: None.

MD_CanTrcv_30_GenericCan_2001:
  Reason: A goto statement is used to improve performance and code size.
  Risk: Code may become hard to understand.
  Prevention: Code inspection.

MD_CanTrcv_30_GenericCan_3201:
  Reason: For some configuration variants code may be unreachable.
  Risk: Code is not executed.
  Prevention: Code inspection and test of the different variants in the component test.

MD_CanTrcv_30_GenericCan_8.7:
  Reason: Definition of CanIfState in global section because of better over-view.
  Risk: Name conflict.
  Prevention: Check for name conflicts.

MD_CanTrcv_30_GenericCan_3673:
  Reason: Depending on the configuration the feature may be unsupported by the underlying CanTrcv hardware.
  Risk: None.
  Prevention: None.

MD_CanTrcv_30_GenericCan_3408:
  Reason: Some local variables are declared without static declaration to allow better debugging.
  Risk: Name conflict.
  Prevention: All external variables are prefixed with <Msn>

MD_CanTrcv_30_GenericCan_3448:
  Reason: Variables are declared with external linkage in order to allow XCP debugging. These must not be accessed
          from outside. So the types shall remain internal.
  Risk: None.
  Prevention: None.

 MD_CanTrcv_30_GenericCan_335X:
  Reason: Caused by High-level Low-level concept of CAN transceiver driver while the High-level must offer maximum flexibility for the Low-level.
          This concept may lead to fact that NOT every condition may be stimulated / required by the Low-level.
  Risk: Code is not executed.
  Prevention: Every High-level + Low-level is tested and reviewed separately.

*/

/* ********************************************************************************************************************
 *  END OF FILE
 * *******************************************************************************************************************/
