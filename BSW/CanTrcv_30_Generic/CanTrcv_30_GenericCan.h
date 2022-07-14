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
 |         File:  CanTrcv_30_GenericCan.h
 |      Project:  MICROSAR
 |       Module:  CAN Transceiver driver
 |    Generator:  DaVinci Configurator 5
 |  Description:  Header of the MICROSAR CAN Transceiver Driver.
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
 *  1.06.00   2008-10-08  Ou      -             No changes, just merge it with the high-level version 1.03
 *  1.07.00   2008-10-28  Ou      -             No changes, just update the specific DLLs
 *  1.08.00   2008-12-11  Ou      -             Use the Autosar compliant copyright
 *  1.09.00   2009-02-19  Tvi     -             Changed to match with updated DLLs
 *  1.10.00   2009-02-26  Tvi     ESCAN00033398 Compiler warnings when using different transceiver types within the same stack
 *  1.11.00   2009-07-01  Fn      -             (no changes here, just to merge with HL)
 *            2009-07-27  Tvi     ESCAN00033879 Define is written wrong and not enclosured with a switch
 *  1.11.01   2009-10-15  Tvi     -             (no changes here, just to merge with HL)
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
 *            GENERICCAN is used for definitions in upper case for defines.
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
 *                        Tvi     ESCAN00049831 [PN only] Store wakeup event and report them if Trcv is in Standby mode
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
#if !defined (CANTRCV_30_GENERICCAN_API_H)
# define CANTRCV_30_GENERICCAN_API_H

/* PRQA S 0850 EOF */ /* MD_MSR_19.8 */
/* PRQA S 0777 EOF */ /* MD_CanTrcv_30_GenericCan_0777 */

# define V_CANTRCV_30_GENERICCAN_HEADER

/* ********************************************************************************************************************
 * INCLUDES
 * *******************************************************************************************************************/
# include "CanTrcv_30_GenericCan_Cfg.h" /* SREQ00010148,SREQ00007523 */

# include "SchM_CanTrcv_30_GenericCan.h"

/* ********************************************************************************************************************
 * Module version
 * *******************************************************************************************************************/

# define CANTRCV_30_GENERICCAN_AR_RELEASE_MAJOR_VERSION      4
# define CANTRCV_30_GENERICCAN_AR_RELEASE_MINOR_VERSION      0
# define CANTRCV_30_GENERICCAN_AR_RELEASE_REVISION_VERSION   3
# define CANTRCV_30_GENERICCAN_AUTOSARVERSION_4X             0x403

/* ##V_CFG_MANAGEMENT ##CQProject : DrvTrans_GenericCandioAsr CQComponent : Implementation */
#define DRVTRANS_GENERICCANDIOASR_VERSION                     0x0401u
#define DRVTRANS_GENERICCANDIOASR_RELEASE_VERSION             0x01u

#if defined(CANTRCV_30_GENERICCAN_INCLUDE_INTERNAL)
/* ##V_CFG_MANAGEMENT ##CQProject : DrvTrans__coreCanxAsr CQComponent : Implementation */
# define DRVTRANS__CORECANXASR_VERSION                         0x0502u
# define DRVTRANS__CORECANXASR_RELEASE_VERSION                 0x01u
#endif

# define CANTRCV_30_GENERICCAN_SW_MAJOR_VERSION               ((DRVTRANS_GENERICCANDIOASR_VERSION & 0xFF00u) >> 8)
# define CANTRCV_30_GENERICCAN_SW_MINOR_VERSION               (DRVTRANS_GENERICCANDIOASR_VERSION & 0x00FFu)
# define CANTRCV_30_GENERICCAN_SW_PATCH_VERSION               DRVTRANS_GENERICCANDIOASR_RELEASE_VERSION

/* ********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 * *******************************************************************************************************************/ 
# define CANTRCV_30_GENERICCAN_VENDOR_ID                      30
# define CANTRCV_30_GENERICCAN_MODULE_ID                      70

/* API IDs for Error Reporting */
# define CANTRCV_30_GENERICCAN_INIT_ID                        0x00
# define CANTRCV_30_GENERICCAN_SETOPMODE_ID                   0x01
# define CANTRCV_30_GENERICCAN_GETOPMODE_ID                   0x02
# define CANTRCV_30_GENERICCAN_GETBUSWUREASON_ID              0x03
# define CANTRCV_30_GENERICCAN_GETVERSIONINFO_ID              0x04
# define CANTRCV_30_GENERICCAN_SETWAKEUPMODE_ID               0x05
# define CANTRCV_30_GENERICCAN_MAINFUNCTION_ID                0x06
# define CANTRCV_30_GENERICCAN_CHECKWAKEUP_ID                 0x07 
# define CANTRCV_30_GENERICCAN_MAINFUNCTIONDIAGNOSTICS        0x08
# define CANTRCV_30_GENERICCAN_GETTRCVSYSTEMDATA_ID           0x09
# define CANTRCV_30_GENERICCAN_CLEARTRCVWUFFLAG_ID            0x0A
# define CANTRCV_30_GENERICCAN_READTRCVTIMEOUTFLAG_ID         0x0B
# define CANTRCV_30_GENERICCAN_CLEARTRCVTIMEOUTFLAG_ID        0x0C
# define CANTRCV_30_GENERICCAN_READTRCVSILENCEFLAG_ID         0x0D
# define CANTRCV_30_GENERICCAN_CHECKWAKEFLAG_ID               0x0E
# define CANTRCV_30_GENERICCAN_SETPNACTIVATIONSTATE_ID        0x0F

# define CANTRCV_30_GENERICCAN_OP_MODE_NORMAL                 CANTRCV_TRCVMODE_NORMAL
# define CANTRCV_30_GENERICCAN_OP_MODE_STANDBY                CANTRCV_TRCVMODE_STANDBY 
# define CANTRCV_30_GENERICCAN_OP_MODE_SLEEP                  CANTRCV_TRCVMODE_SLEEP

# define CANTRCV_30_GENERICCAN_WU_ENABLE                      CANTRCV_WUMODE_ENABLE
# define CANTRCV_30_GENERICCAN_WU_DISABLE                     CANTRCV_WUMODE_DISABLE
# define CANTRCV_30_GENERICCAN_WU_CLEAR                       CANTRCV_WUMODE_CLEAR

# define CANTRCV_30_GENERICCAN_WU_ERROR                       CANTRCV_WU_ERROR
# define CANTRCV_30_GENERICCAN_WU_NOT_SUPPORTED               CANTRCV_WU_NOT_SUPPORTED
# define CANTRCV_30_GENERICCAN_WU_BY_BUS                      CANTRCV_WU_BY_BUS
# define CANTRCV_30_GENERICCAN_WU_INTERNALLY                  CANTRCV_WU_INTERNALLY
# define CANTRCV_30_GENERICCAN_WU_RESET                       CANTRCV_WU_RESET
# define CANTRCV_30_GENERICCAN_WU_POWER_ON                    CANTRCV_WU_POWER_ON
# define CANTRCV_30_GENERICCAN_WU_BY_PIN                      CANTRCV_WU_BY_PIN
# define CANTRCV_30_GENERICCAN_WU_BY_SYSERR                   CANTRCV_WU_BY_SYSERR

# define CANTRCV_30_GENERICCAN_E_NO_ERROR                     0x00
# define CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK          0x01
/* SREQ00010156: Keep the invalid CAN Network switch for compatibility reason. */
# define CANTRCV_30_GENERICCAN_E_INVALID_TRANSCEIVER         CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK
# define CANTRCV_30_GENERICCAN_E_PARAM_POINTER                0x02
# define CANTRCV_30_GENERICCAN_E_UNINIT                       0x11
# define CANTRCV_30_GENERICCAN_E_TRCV_NOT_STANDBY             0x21
# define CANTRCV_30_GENERICCAN_E_TRCV_NOT_NORMAL              0x22
# define CANTRCV_30_GENERICCAN_E_PARAM_TRCV_WAKEUP_MODE       0x23
# define CANTRCV_30_GENERICCAN_E_PARAM_TRCV_OP_MODE           0x24
# define CANTRCV_30_GENERICCAN_E_BAUDRATE_NOT_SUPPORTED       0x25
# define CANTRCV_30_GENERICCAN_E_CLR_TRCV_WUF_NOT_CALLED      0x30
# if defined(CANTRCV_30_GENERICCAN_AUTOSARVERSION_4X)
#  define CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL             0x40 /* SREQ00010156: Actual ID is unspecified in SWS! */
# endif

# define CANTRCV_30_GENERICCAN_WAKEUP_NOT_SUPPORTED          0
# define CANTRCV_30_GENERICCAN_WAKEUP_BY_POLLING             1
/* Compatibility defines... */
# define CANTRCV_30_GENERICCAN_WAKEUP_NONE                   CANTRCV_30_GENERICCAN_WAKEUP_NOT_SUPPORTED
# define CANTRCV_30_GENERICCAN_WAKEUP_POLLING                CANTRCV_30_GENERICCAN_WAKEUP_BY_POLLING

# define CanTrcv_30_GenericCan_TrcvModeType                  CanTrcv_TrcvModeType
# define CanTrcv_30_GenericCan_TrcvWakeupModeType            CanTrcv_TrcvWakeupModeType
# define CanTrcv_30_GenericCan_TrcvWakeupReasonType          CanTrcv_TrcvWakeupReasonType

# if defined(CANTRCV_30_GENERICCAN_USE_ICU)
# else
#  define CANTRCV_30_GENERICCAN_USE_ICU                       STD_OFF
# endif

/* Timer configuration */
/* TimerConfiguration
   ------------------

   Activate this to enable the usage of hardware timers. For the Tja1041
   these timers are needed to wait some time after some mode transitions.
   
   Activating this will enable the callback:
      Appl_CanTrcv_30_GenericCan_Wait(uint8 TimerIndex)

   Example TJA1041:
   # define CANTRCV_30_GENERICCAN_USE_TIMERS                    STD_ON
*/
<Your_Trcv_Code>

/* Predefined timer indexes (0x01 - 0x0f)                                   */
# define kCanTrcv_30_GenericCan_LoopInit                          0x01

/*  AdditionalTimer
    ---------------
    
    If needed, you may specify additional timer indexes here. These must have
    the following format and can be used within Appl_CanTrcv_30_GenericCan_Wait

    # define kCanTrcv_30_GenericCan_<identifier>                 <0x10 - 0xff>
*/
<Your_Trcv_Code>

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 * *******************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 * *******************************************************************************************************************/
/* SREQ00007522 */
typedef enum
{
  CANTRCV_30_GENERICCAN_FLAG_SET,
  CANTRCV_30_GENERICCAN_FLAG_CLEARED
}
CanTrcv_30_GenericCan_TrcvFlagStateType;

typedef enum
{
  CANTRCV_30_GENERICCAN_PN_ENABLED,
  CANTRCV_30_GENERICCAN_PN_DISABLED
} CanTrcv_30_GenericCan_PNActivationType;

/* ********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 * *******************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 * *******************************************************************************************************************/
/* for callback function prototypes see CanTrcv_Cbk.h */
# define CANTRCV_30_GENERICCAN_START_SEC_CODE
# include "MemMap.h"

/************************************************************************************************************
 | NAME:  CanTrcv_30_GenericCan_Init()
 ***********************************************************************************************************/
/*! \brief         Initializes the CAN transceiver module.
 *  \details       This function initializes all active channels of the CAN transceiver driver which are configured in the configuration tool.
 *                 This function has the service id 0x00.
 *  \param[in]     ConfigPtr - Pointer to the CanTrcv_30_GenericCan_Config structure.
 *                 If multiple configurations are available, the active configuration can be selected by using the related CanTrcv_30_GenericCan_Config_<IdentityName> structure.
 *  \return        -
 *  \pre           The function CanTrcv_30_GenericCan_InitMemory must be called before the function CanTrcv_30_GenericCan_Init can be called.
 *                 This function must be called before any other service functionality of the transceiver driver
 *                 The SPI/DIO driver must be initialized
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
 *  \trace         CREQ-102476, CREQ-102495, CREQ-102482, CREQ-102496, CREQ-102497, CREQ-102498, CREQ-102501,
 *                 CREQ-102500
***********************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_Init( P2CONST(CanTrcv_30_GenericCan_ConfigType, AUTOMATIC, CANTRCV_30_GENERICCAN_CONST) ConfigPtr );

/***********************************************************************************************************************
 | NAME:  CanTrcv_30_GenericCan_InitMemory
 **********************************************************************************************************************/
/*! \brief         Initialization of memory of the CAN transceiver module.
 *  \details
 *  \param[in]     -
 *  \return        -
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
 *  \trace         CREQ-102476, CREQ-102496, CREQ-102497
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_InitMemory(void);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetOpMode
 **********************************************************************************************************************/
/*! \brief       Change the transceiver state into the requested mode.
 *  \details     This function has the service id 0x01.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[in]   OpMode: Requested mode to which the transceiver state has to be changed.
 *  \return      E_OK: If transceiver state was changed into the requested mode.
 *  \return      E_NOT_OK: If transceiver state failed to change or mode parameter is invalid.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      -
 *  \trace       CREQ-102495, CREQ-102474, CREQ-102482, CREQ-102487, CREQ-102477, CREQ-102478, CREQ-102479, CREQ-102496, 
 *               CREQ-102497, CREQ-102498, CREQ-102501, CREQ-102500
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetOpMode(uint8 CanTrcvIndex, CanTrcv_TrcvModeType OpMode);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetOpMode
 **********************************************************************************************************************/
/*! \brief       Detect the current transceiver operation mode and store it.
 *  \details     DIO transceiver: The mode is determined from status of PINs of the transceiver hardware.
 *               SPI transceiver: The mode is read from state variable. No access to transceiver hardware is performed.
 *               This function has the service id 0x02.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[in]   OpMode: Pointer to buffer where the current operation mode is stored.
 *  \return      E_OK: If current transceiver operation mode was detected.
 *  \return      E_NOT_OK: If transceiver operation mode is not initialized.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \trace       CREQ-102495, CREQ-102475, CREQ-102477, CREQ-102478, CREQ-102479, CREQ-102496, CREQ-102497, CREQ-102498, 
 *               CREQ-102501, CREQ-102500
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetOpMode(uint8 CanTrcvIndex, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) OpMode);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetBusWuReason
 **********************************************************************************************************************/
/*! \brief       Detect and store the bus wake-up reason.
 *  \details     The wake-up reason is read from state variable. No access to transceiver hardware is performed.
 *               This function has the service id 0x03.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[in]   Reason: Pointer to buffer where the bus wake-up reason is stored.
 *  \return      E_OK: If bus wake-up reason was detected.
 *  \return      E_NOT_OK: If bus wake-up reason is not detected or feature is not supported.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \trace       CREQ-102495, CREQ-102484, CREQ-102496, CREQ-102497
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetBusWuReason(uint8 CanTrcvIndex, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) Reason);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetWakeupMode
 **********************************************************************************************************************/
/*! \brief       Change the wake-up mode to the requested mode.
 *  \details     For more information about possible wake-up modes please see the description of type <CanTrcv_TrcvWakeupModeType>.
 *               This function has the service id 0x05.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[in]   TrcvWakeupMode: Requested wake-up mode for the transceiver channel (CanTrcvIndex).
 *  \return      E_OK: If wake-up mode is changed to the requested mode.
 *  \return      E_NOT_OK: If the wake-up mode change has failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      -
 *  \trace       CREQ-102495, CREQ-102483, CREQ-102496, CREQ-102497
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetWakeupMode(uint8 CanTrcvIndex, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckWakeup
 **********************************************************************************************************************/
/*! \brief       Checks if a wake-up event was detected.
 *  \details     This function has the service id 0x07.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      E_OK: If a wake-up was detected / the request to check for wake-up was accepted (only asynchronous SPI-interface).
 *  \return      E_NOT_OK: If NO wake-up was detected / the request to check for wake-up was rejected (only asynchronous SPI-interface).
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      -
 *  \trace       CREQ-102495, CREQ-102480, CREQ-102502, CREQ-102496, CREQ-102497, CREQ-102498, CREQ-102501, CREQ-102500
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckWakeup(uint8 CanTrcvIndex);

#if (CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief       Get the version info of the CAN transceiver module
 *  \details     The versions are stored into the structure pointed by VersionInfo.
 *               This function has the service id 0x04.
 *  \param[in]   VersionInfo: Structure pointer for storing of version info
 *  \return      -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON
 *  \trace       CREQ-102494, CREQ-102495, CREQ-102496, CREQ-102497
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) VersionInfo);
#endif /* (CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON) */

/* SREQ00007670 */

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_MainFunction
 **********************************************************************************************************************/
/*! \brief       Service to scan all buses for wake up events and perform these event.
 *  \details     This function has the service id 0x06.
 *  \param[in]   -
 *  \return      -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      -
 *  \trace       CREQ-102481, CREQ-102496, CREQ-102497, CREQ-102498, CREQ-102501, CREQ-102500
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_MainFunction(void);

#  if (CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON) /* SREQ00007558 */
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_MainFunctionDiagnostics
 **********************************************************************************************************************/
/*! \brief       Reads the transceiver diagnostic status periodically and sets product/development error accordingly.
 *  \details     This function has the service id 0x08.
 *  \param[in]   -
 *  \return      -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON
 *  \trace       CREQ-102492, CREQ-102496, CREQ-102497, CREQ-102501, CREQ-102500
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_MainFunctionDiagnostics(void);
#  endif /* CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON */

# define CANTRCV_30_GENERICCAN_STOP_SEC_CODE
# include "MemMap.h"

#endif /* CANTRCV_30_GENERICCAN_API_H */

/* ********************************************************************************************************************
 *  END OF FILE
 * *******************************************************************************************************************/
